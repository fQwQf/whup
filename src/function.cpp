#include "function.h"
#include "expression.h"
#include "check.h"
#include "WHUPstream.h"

extern WHUPstream_compile1 WHUPout;

extern std::vector<Error> errors;

std::string function_ret_label;    // 只有在处理函数时才会有的值。用于函数返回时跳转至ret区域。
std::string function_return_value; // 同理
std::string function_return_type;

extern std::unordered_map<std::string, Function *> functions;     // 存储函数名和对应的对象指针哈希表
extern std::unordered_map<std::string, std::string> var_declares; // 存储将放入c++中变量名和类型的哈希表
extern std::vector<ThreeAddressCode> tacs;                        // 存储三地址代码的向量

std::set<Function*> used_functions;

Function::Function() {
    // 这东西什么用都没有，但是必须要有，否则编译器无法自动生成ClassFunction的默认构造函数
};

Function::Function(std::vector<Token> &tokens, Environment *env)
{
    start_label = newTempLabel();
    end_label = newTempLabel();
    this->env = new Environment(env); // 函数自己的环境

    int i = 0;
    if (tokens[0].type == KEYWORD && tokens[0].value == "function") // 若是function关键字，则跳过
    {
        i++;
    }

    if (tokens[i].type == IDENTIFIER && tokens[i + 1].type == SYMBOL && tokens[i + 1].value == "(")
    {
        name = tokens[i].value;
        functions[name] = this; // 将函数插入全局函数表
        env->function_table[name] = this; // 将函数插入环境函数表
        i++;
    }

    tokens.erase(tokens.begin(), tokens.begin() + i);

    WHUPout << "Function name: " << name << std::endl;

    // 1.形参处理
    folmalPara(tokens);

    // 2.分析返回类型
    returnType(tokens);

    // 3.分析函数体
    bodyTokens(tokens);
}

void Function::matchPar(int &i, std::vector<Token> &tokens) // 实际上是由marchBrace改过来的
{
    if (tokens[i].value == "(")
    {
        int leftPar = 1;
        int rightPar = 0; // 分别记录已经读取的左大括号右大括号的个数,当相等时即可结束
        while (leftPar != rightPar)
        {
            ++i;
            if (tokens[i].value == ")")
            {
                rightPar++;
            }
            else if (tokens[i].value == "(")
            {
                leftPar++;
            }
            else
                continue;
        }
    }
}

std::string Function::call(std::vector<Token> &tokens, Environment *env)
{ // 返回值是储存返回值的临时变量名
    WHUPout << "call function " << name << std::endl;
    for (auto &i : tokens)
    {
        WHUPout << i.value << " ";
    }
    WHUPout << std::endl;

    //参数入栈
    push_real_para(env);

    // 现在开始处理参数，具体来说，根据逗号，将参数分为多个subtokens，然后传入expression，最后将结果赋给形参
    // 实际上这一块和block扫描statement的逻辑类似，所以我直接把block拿过来修改一下就成了🙂‍↕️
    // 要把形参在local_env中登记
    int i = 1;
    this->matchPar(i, tokens);

    // 这是去掉括号外部分的tokens
    std::vector<Token> realParaTokens = std::vector<Token>(tokens.begin() + 1, tokens.begin() + i + 1);

    this->realPara(realParaTokens, env);

    call_with_stack_frame(env);

    WHUPout << "call function " << name << " success" << std::endl;


    return return_value;
}

void Function::push_real_para(Environment *env)
{
    if (env->isGlobal())
    { //如果在全局环境中调用，则不需要保存栈帧
        return;
    }
    else
    {
        for (auto &i : params_name)
        {
            tacs.push_back({PUSH,"push", i.second, "", ""});
        }
    }
}

void Function::call_with_stack_frame(Environment *env)
{
    used_functions.insert(this);

    if (env->isGlobal())
    { //如果在全局环境中调用，则不需要保存栈帧
        tacs.push_back({CALL,"call", start_label, "", ""});
        return;
    }
    else
    {
        std::vector<std::string> stack_frame;
        
        //参数会在调用时改变，应该提前压入

        //将局部变量加入栈帧
        for (auto &i : env->var_table)
        {
            WHUPout << "local var: " << i.first << " " << i.second << std::endl;
            stack_frame.push_back(env->get_var(i.first));
        }

        //函数调用返回值加入栈帧
        stack_frame.insert(stack_frame.end(), env->return_var_list.begin(), env->return_var_list.end());

        //输出函数调用返回值
        WHUPout << "env->return_var_list: ";
        for (auto &i : env->return_var_list)
        {
            WHUPout << i << " ";
        }
        WHUPout << std::endl;


        //将栈帧压入栈中
        for(auto &i : stack_frame){
            tacs.push_back({PUSH,"push",i,"",""});
        }

        tacs.push_back({CALL,"call", start_label, "", ""});

        //恢复栈帧
        for(int i=stack_frame.size()-1;i>=0;i--){
            tacs.push_back({POP,"pop","","",stack_frame[i]});
        }
        for(int i=params_name.size()-1;i>=0;i--){
            tacs.push_back({POP,"pop","","",params_name[i].second});
        }
    }
}

void Function::generate()
{

    body_tokens.erase(body_tokens.begin(), body_tokens.begin() + 1); // 去掉: type {
    //防止不写冒号直接写返回值的情况
    if(body_tokens[0].value=="{"){
        body_tokens.erase(body_tokens.begin());
    }

    body_tokens.pop_back();                                          // 去掉 }

    WHUPout << "Function: " << name << std::endl;
    WHUPout << "Params: " << std::endl;
    for (auto &param : params_name)
    {
        WHUPout << "  " << param.first << " " << param.second << std::endl;
    }
    WHUPout << "Return type: " << return_type << std::endl;
    WHUPout << "Body: " << std::endl;
    for (auto &token : body_tokens)
    {
        WHUPout << "  " << token.value;
    }
    WHUPout << std::endl;

    function_ret_label = end_label;
    function_return_value = return_value;
    function_return_type = return_type;
    tacs.push_back({LABEL,"label", "", "", start_label});

    // Block能否识别临时变量？
    new Block(body_tokens, env);

    //如果之前没有return，则在最后自动return
    tacs.push_back({RET,"return", "", "", ""});
}

// void Function::generateInline()
// {
//     body_tokens.erase(body_tokens.begin(), body_tokens.begin() + 1); // 去掉: type {
//     body_tokens.pop_back();                                          // 去掉 }

//     WHUPout << "Function: " << name << std::endl;
//     WHUPout << "Params: " << std::endl;
//     for (auto &param : params_name)
//     {
//         WHUPout << "  " << param.first << " " << param.second << std::endl;
//     }
//     WHUPout << "Return type: " << return_type << std::endl;
//     WHUPout << "Body: " << std::endl;
//     for (auto &token : body_tokens)
//     {
//         WHUPout << "  " << token.value;
//     }
//     WHUPout << std::endl;


//     // Block能否识别临时变量？
//     new Block(body_tokens, env);
// }

////////////////////////////////
// 尝试对classfunction进行模块化//
////////////////////////////////

// 将括号一起传入
void Function::folmalPara(std::vector<Token> &tokens)
{
    // 1.形参处理
    //  现在开始分析形参
    //  TODO:如果这里发现错误如首token不是括号，抛出异常
    int i = 0;
    if (tokens[0].type == SYMBOL && tokens[0].value == "(")
    {
        // tokens.erase(tokens.begin());
        i++;

        // 接下来要按照逗号和冒号来分割参数
        while (tokens[i].type != SYMBOL || tokens[i].value != ")")
        {
            if (tokens[i].type == SYMBOL && tokens[i].value == ",")
            {
                // tokens.erase(tokens.begin());
                i++;
            }
            std::string param_name = tokens[i].value;
            // tokens.erase(tokens.begin());
            i++;
            if (tokens[i].type == SYMBOL && tokens[i].value == ":")
            {
                // tokens.erase(tokens.begin());
                i++;
                paramType pt;
                pt.type=tokens[i].value;
                params_name.push_back({param_name, newTempVar(tokens[i].value)});
                
                i++;
                // tokens.erase(tokens.begin(), tokens.begin() + 1);
                pt.isreference=false;
                if(tokens[i].value=="&")
                {
                    WHUPout<<"&"<<std::endl;
                    WHUPout<<tokens[i].value<<std::endl;
                    pt.isreference=true;
                    i++;
                }

                params_type.push_back(pt);
            }
            else
            {
                pushErrors(tokens[0], "No type for parameter " + param_name);
            }
        }
        // tokens.erase(tokens.begin());
        i++;
    }

    // 现在登记参数
    for (int param_num = 0; param_num < params_name.size(); param_num++)
    {
        env->insert_var(params_name[param_num].first);
        env->change_type_var(params_name[param_num].first, params_type[param_num].type);
    }

    for(int i=0;i<params_name.size();i++)
    {
        WHUPout<<"param "<<params_name[i].first<<" is "<<params_name[i].second<<std::endl;
        WHUPout<<"param "<<params_name[i].first<<" type is "<<params_type[i].type<<std::endl;
        WHUPout<<"param "<<params_name[i].first<<" isreference is "<<params_type[i].isreference<<std::endl;
        WHUPout<<std::endl;
    }
    tokens.erase(tokens.begin(), tokens.begin() + i);
}

// 完全可以根据函数定义是否有返回值来判断是否调用这个函数
// 有的化接受“：type”这种形式，没有的话直接跳过
void Function::returnType(std::vector<Token> &tokens)
{
    // 2.函数返回值处理
    //  这里分析返回类型
    int i = 0;
    if (tokens[0].type == SYMBOL && tokens[0].value == ":")
    {
        i++;
        if(tokens[i].value=="{")
            pushErrors(tokens[0], "Need return type before colon for function " + name);
        return_type = tokens[i].value;
        return_value = newTempVar(return_type);
        i++;
        // 得到return_type和return_value
    }
    else
    {
        // return_type = "void";
        // return_value = "";
        if(tokens[i].value!="{")
            pushErrors(tokens[0], "Missing colon before return type for function " + name);
        for (int i = 0; i < tokens.size(); i++)
        {
            if (tokens[i].type == KEYWORD && tokens[i].value == "return")
            {
                if (tokens[i + 1].type == SYMBOL && tokens[i + 1].value == ";")
                {
                    continue;
                }
                else
                {
                    pushErrors(tokens[i], "Missing return type for function " + name);
                }
            }
        }
    }

    tokens.erase(tokens.begin(), tokens.begin() + i);

    //
}

void Function::realPara(std::vector<Token> &tokens, Environment *env)
{
    // 仍然将整个括号传入
    if (tokens[0].value != "(")
    {
        pushErrors(tokens[0], "Not a function ");
        return;
    }
    int index = 1;
    // 5.实参处理
    // 现在开始处理参数，具体来说，根据逗号，将参数分为多个subtokens，然后传入expression，最后将结果赋给形参
    // 实际上这一块和block扫描statement的逻辑类似，所以我直接把block拿过来修改一下就成了🙂‍↕️
    // 要把形参在local_env中登记
    int last_comma = index;
    int param_num = 0;

    //下面的过程只能识别后面接,的参数，于是就这样
    tokens[tokens.size()-1].value = ",";

    for (int i = index; i < tokens.size(); i++)
    {
        matchPar(i, tokens);
        if (tokens[i].type == SYMBOL && (tokens[i].value == ","))
        {
            std::vector<Token> subtokens(tokens.begin() + last_comma, tokens.begin() + i);
            last_comma = i + 1;
            Expr *expression = new Expr(subtokens, env);
            WHUPout << "pass value success!!!" << std::endl;
            if(!params_type[param_num].isreference)//按值
            {
                if(expression->return_type()=="string")
                tacs.push_back({STRASSIGN,"=", expression->getTacResult(), "", params_name[param_num].second});
                else
                tacs.push_back({ASSIGN,"=", expression->getTacResult(), "", params_name[param_num].second});
            }
            else//按引用
            {
                WHUPout<<std::endl<<"reference"<<std::endl<<std::endl;
                if(expression->return_type()=="string")
                tacs.push_back({REFSTR,"=", expression->getTacResult(), "", params_name[param_num].second});
                else
                tacs.push_back({REFNUM,"=", expression->getTacResult(), "", params_name[param_num].second});
            }
            WHUPout << "param " << params_name[param_num].first << " is " << params_name[param_num].second << std::endl;

            param_num += 1;
        }
    }

    //
}

void Function::bodyTokens(std::vector<Token> &tokens)
{
    // 3.函数体处理
    //  这里分析函数体
    //  函数体分析可以直接继承Block,因为函数体就是一段代码块
    //  只需要对Token进行修改，改变其中的参数名即可，改成env处理后的
    // 注意包含括号
    int i = 0;
    for (auto &token : tokens)
    {
        if (token.type == IDENTIFIER)
        {
            for (int i = 0; i < params_name.size(); i++)
            {
                if (token.value == params_name[i].first)
                {
                    token.value = params_name[i].second; // 这里的params_name[i].second是形参对应的临时变量名
                    token.processed = true;
                    WHUPout << "Processed token: " << params_name[i].first << " to " << token.value << std::endl;
                    break;
                }
                else
                {
                    token.processed = false;
                }
            }
        }
    }

    this->body_tokens = tokens;
    WHUPout << this->name << " body tokens:" << std::endl;
    for (auto &i : body_tokens)
    {
        WHUPout << i.value << " ";
    }
}

std::string Function::get_return_value()
{
    return return_value;
}