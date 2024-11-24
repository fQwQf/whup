#include"classfunction.h"
#include"expression.h"

extern std::string function_ret_label;//只有在处理函数时才会有的值。用于函数返回时跳转至ret区域。
extern std::string function_return_value;//同理
extern std::vector<ThreeAddressCode> tacs;

//处理类函数声明时，需要传入类函数表和类环境
ClassFunction::ClassFunction(std::vector<Token> &tokens,Environment *env,std::unordered_map<std::string,ClassFunction*>& function_table)
{
    start_label = newTempLabel();
    end_label = newTempLabel();
    jump_in_label = newTempVar("string");
    this->ClassFunction_env = new Environment(env);//函数自己的环境,且用类的环境初始化，要用类的环境来处理参数

    tokens.erase(tokens.begin()); // 删除第一个token，即function关键字
    name = tokens[0].value;
    tokens.erase(tokens.begin());

    function_table[name] = this;//将函数插入类函数表//这里与普通函数不同，普通函数是插入全局函数表
    std::cout << "Function name: " << name << std::endl;

    // 现在开始分析形参
    // TODO:如果这里发现错误如首token不是括号，抛出异常
    if (tokens[0].type == SYMBOL && tokens[0].value == "(")
    {
        tokens.erase(tokens.begin());
        // 接下来要按照逗号和冒号来分割参数
        while (tokens[0].type != SYMBOL || tokens[0].value != ")")
        {
            std::string param_name=tokens[0].value;
            tokens.erase(tokens.begin());
            if (tokens[0].type == SYMBOL && tokens[0].value == ",")
            {
                params_type.push_back("auto");
                params_name.push_back({param_name, newTempVar("auto")});
                tokens.erase(tokens.begin());
            }
            else if (tokens[0].type == SYMBOL && tokens[0].value == ":")
            {
                tokens.erase(tokens.begin());
                params_type.push_back(tokens[0].value);
                params_name.push_back({param_name, newTempVar(tokens[0].value)});

                tokens.erase(tokens.begin(), tokens.begin() + 1);
            }
        }
        tokens.erase(tokens.begin());
    }

    // 这里分析返回类型
    if (tokens[0].type == SYMBOL && tokens[0].value == ":")
    {
        tokens.erase(tokens.begin());
        return_type = tokens[0].value;
        return_value = newTempVar(return_type);
        tokens.erase(tokens.begin());
    }
    else
    {
        std::cout << "No return type for function " << name << std::endl;
    }

    //现在登记参数
    for (int param_num = 0; param_num < params_name.size(); param_num++)
    {
        ClassFunction_env->insert_var(params_name[param_num].first);
        ClassFunction_env->change_type_var(params_name[param_num].first, params_type[param_num]);
    }

    // 这里分析函数体
    // 函数体分析可以直接继承Block,因为函数体就是一段代码块
    // 只需要对Token进行修改，改变其中的参数名即可，改成env处理后的
    for (auto &token : tokens)
    {
        if (token.type == IDENTIFIER)
        {
            for (int i = 0; i < params_name.size(); i++)
            {
                if (token.value == params_name[i].first)
                {
                    token.value = params_name[i].second;
                    token.processed = true;
                    std::cout << "Processed token: " << params_name[i].first << " to " << token.value << std::endl;
                    break;
                }
                else{
                     token.processed = false;
                }
            }
        }
    }

    this->body_tokens = tokens;
    for(auto&i:body_tokens)
    {
        std::cout<<i.value<<" ";
    }
}

void ClassFunction::matchPar(int &i,std::vector<Token> &tokens)//实际上是由marchBrace改过来的
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


//与普通函数相比调用的时候，要多一步对实例名的识别，这样才能找到对应的函数表
std::string ClassFunction::call(std::vector<Token> &tokens,Environment* env){//返回值是储存返回值的临时变量名
    std::cout<<"call function "<<name<<std::endl;
    for(auto&i:tokens)
    {
        std::cout<<i.value<<" ";
    }
    std::cout<<std::endl;
    tokens.erase(tokens.begin(),tokens.begin()+3);//去掉实例名，"->",和函数名


    //现在开始处理参数，具体来说，根据逗号，将参数分为多个subtokens，然后传入expression，最后将结果赋给形参
    //实际上这一块和block扫描statement的逻辑类似，所以我直接把block拿过来修改一下就成了🙂‍↕️
    //要把形参在local_env中登记
    int last_comma = 0;
    int param_num = 0;
    tokens.erase(tokens.begin());
    

    
    for (int i = 0; i < tokens.size(); i++)
    {
        matchPar(i, tokens);
        if (tokens[i].type == SYMBOL && (tokens[i].value == "," || tokens[i].value == ")"))
        {
            std::vector<Token> subtokens(tokens.begin() + last_comma, tokens.begin() + i);
            last_comma = i+1;
            Expr* expression = new Expr(subtokens,env);
            tacs.push_back({"=",expression->getTacResult(),"",params_name[param_num].second});
            std::cout << "param " << params_name[param_num].first << " is " << params_name[param_num].second << std::endl;

            param_num+=1;
        }
        //呃呃有点没懂
        if (tokens[i].type == SYMBOL && tokens[i].value == ")")
        {
            tokens.erase(tokens.begin(),tokens.begin() + i);//检测到括号，则删除括号及括号之前的所有内容
            break;
        }
    }

    //现在应该设置跳转，即一个跳出的if...goto...，一个用于跳回的label
    std::string label = newTempLabel();
    tacs.push_back({"=","\""+label+"\"","",jump_in_label});
    tacs.push_back({"if_goto","true","",start_label});
    tacs.push_back({"label","","",label});
    return_labels.push_back(label);

    std::cout<<"call function "<<name<<" success"<<std::endl;
    return return_value;


}


void ClassFunction::generate(){

    body_tokens.erase(body_tokens.begin(), body_tokens.begin() + 1);//去掉: type {
    body_tokens.pop_back();//去掉 }

    std::cout << "Function: " << name << std::endl;
    std::cout << "Params: " << std::endl;
    for (auto &param : params_name)
    {
        std::cout << "  " << param.first << " " << param.second << std::endl;
    }
    std::cout << "Return type: " << return_type << std::endl;
    std::cout << "Body: " << std::endl;
    for (auto &token : body_tokens)
    {
        std::cout << "  " << token.value;
    }
    std::cout << std::endl;


    function_ret_label = end_label;
    function_return_value = return_value;
    tacs.push_back({"label","","",start_label});
    new Block(body_tokens,ClassFunction_env);

    //以下是跳转区
    tacs.push_back({"label","","",end_label});
    for(auto i:return_labels){
        std::string bool_var = newTempVar("bool");
        tacs.push_back({"==",jump_in_label,"\""+i+"\"",bool_var});
        tacs.push_back({"if_goto",bool_var,"",i});
    }
}

std::string ClassFunction::get_return_value(){
    return return_value;
}
