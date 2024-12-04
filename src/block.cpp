#include "block.h"
#include"assign.h"
#include"var.h"
#include"print.h"
#include"if.h"
#include"while.h"
#include"break.h"
#include"continue.h"
#include "return.h"
#include "function.h"
#include "whup_parser.h"
#include"class.h"
#include"object.h"
#include"classfunction.h"
#include "check.h"
#include "whup_io.h"

extern std::string function_ret_label; // 函数返回标签，可用于检测是否在处理函数。
extern std::unordered_map<std::string, Function*> functions;  // 存储所有函数名和对应的对象指针哈希表，用于生成函数体
extern std::vector<std::unordered_map<std::string,ClassFunction*>> all_Object_function_table;
extern std::unordered_map<std::string,Object*>object_table;
extern std::unordered_map<std::string,Class*>class_table;
extern std::vector<Error> errors; // 存储错误信息
extern std::unordered_map<std::string, Environment*> namespace_table; // 存储命名空间名和对应的Environment对象的哈希表

//跳过大括号
void Block::matchBrace(int &i,std::vector<Token> &tokens)
{
    if (tokens[i].value == "{")
    {
        int leftPar = 1;//甚至par都没有改成brace
        int rightPar = 0; // 分别记录已经读取的左大括号右大括号的个数,当相等时即可结束
        while (leftPar != rightPar)
        {
            ++i;
            if (tokens[i].value == "}")
            {
                rightPar++;
            }
            else if (tokens[i].value == "{")
            {
                leftPar++;
            }
            else
                continue;
        }
    }
}

Block::Block(std::vector<Token> tokens, Environment *e)
{
    this->env = new Environment(e);

    block(tokens);
}

Block::Block(std::vector<Token> tokens)//这个是全局block
{
    this->env = new Environment();

    

    block(tokens);

    tacs.push_back(ThreeAddressCode{EXIT,"exit", "", "", ""});

    for(auto&object:object_table)
    {
        object.second->myConstructor->generate();
    }

    for(auto&funcTable:all_Object_function_table)
    {
        for(auto&i:funcTable)
        {
            i.second->generate();
        }
    }
    for (auto &i : functions){
        i.second->generate();
    }

    //先将各个实例的构造函数函数体生成出来
    //这个过程中会将所有数据成员的类型确定下来
    //避免其他成员函数中使用的变量未确定类型，导致函数体中出现null
    
}

// 以分号为分隔扫描
void Block::block(std::vector<Token> tokens)
{
    int last_semicolon = 0;

    for (int i = 0; i < tokens.size(); i++)
    {
        
        matchBrace(i, tokens);
        if (tokens[i].type == SYMBOL && tokens[i].value == ";")
        {
            std::vector<Token> subtokens(tokens.begin() + last_semicolon, tokens.begin() + i);
            // 打印出所有Token
            // debug时可能有用
            std::cout<<"subtokens:"<<std::endl;
            for(auto &i:subtokens){
               std::cout << i.value << " ";
            }
            std::cout << std::endl;
            
            last_semicolon = i+1;
            generate(subtokens);
        }
    }

    return;
}

// 根据首token传入对应的类的构造函数中。
void Block::generate(std::vector<Token> subtokens)
{


    if (subtokens.empty())
        return;

    std::vector<Token> namespace_tokens;

    if (subtokens[0].type == IDENTIFIER && subtokens[1].type == SYMBOL && subtokens[1].value == "::") {
        // 对于命名空间，先转化为没有命名空间的情况
        namespace_tokens = std::vector<Token>(subtokens.begin(), subtokens.begin()+2);
        subtokens.erase(subtokens.begin(), subtokens.begin()+2);
    }


    if (subtokens[0].type == IDENTIFIER && subtokens[1].value != "("&&subtokens[1].type!=IDENTIFIER&&subtokens[1].value!="->")
    {
        std::cout<<"assign begin"<<std::endl;
        subtokens.insert(subtokens.begin(),namespace_tokens.begin(),namespace_tokens.end());

        for(auto&i:subtokens)
        {
            std::cout<<i.value<<" ";
        }
        std::cout<<std::endl;

        //新增对类的赋值语句，只支持object1=object2这样的语句
        if(object_table.find(subtokens[0].value)!=object_table.end())
        {
            object_table[subtokens[0].value]->copy(object_table[subtokens[2].value]);
        }
        else
        new Assign(subtokens,env);
        std::cout << "assign generate" << std::endl;
    }
    else if(subtokens[0].type == IDENTIFIER && subtokens[1].value == "(")
    {
        Environment* funcenv = this -> env;
        //如果指定了命名空间，切过去
        if(!namespace_tokens.empty() && namespace_tokens[0].type == IDENTIFIER){
            //TODO 这里加个找不到命名空间的报错
            funcenv = namespace_table[namespace_tokens[0].value];
        }

        if (funcenv->get_function(subtokens[0].value) == nullptr)
        {
            //TODO 改成报错
            std::cout << "Function " << subtokens[0].value << " not found" << std::endl;
        }
        else
        {
            Function *func = funcenv->get_function(subtokens[0].value);
            func->call(subtokens, env);
            std::cout << "call function: " << subtokens[0].value << std::endl;
            return;
        }
    }
    else if (subtokens[0].type == KEYWORD && subtokens[0].value == "var")
    {
        new Var(subtokens,env);
        std::cout << "var generate" << std::endl;
    }
    else if (subtokens[0].type == KEYWORD && subtokens[0].value == "print")
    {
        new Print(subtokens,env);
        std::cout << "print generate" << std::endl;
    }
    else if(subtokens[0].type==KEYWORD && subtokens[0].value=="if")
    {
        new If(subtokens,env);
    }
    else if(subtokens[0].type==KEYWORD && subtokens[0].value=="while")
    {
        new While(subtokens,env);
    }
    else if(subtokens[0].type==KEYWORD && subtokens[0].value=="break")
    {
        new Break(env);
    }
    else if(subtokens[0].type==KEYWORD&&subtokens[0].value=="continue")
    {
        new Continue(env);
    }
    else if(subtokens[0].type==KEYWORD&&subtokens[0].value=="return")
    {
        if (function_ret_label == "")
        {
            pushErrors(subtokens[0],"unexpected retrun");
            // exit(1);
        }
        else
        {
            if (subtokens.size() == 1)
            {
                new Return(env);
                std::cout << "find return!" << std::endl;
            }
            else
            {
                new Return(subtokens, env);
                std::cout << "find return!" << std::endl;
            }
        }
    }
    else if(subtokens[0].type==KEYWORD&&subtokens[0].value=="function")
    {
        new Function(subtokens,env);
    }
    else if(subtokens[0].type==KEYWORD&&subtokens[0].value=="class")
    {
        new Class(subtokens);
    }
    // else if(subtokens[0].type==IDENTIFIER&&subtokens[1].type==IDENTIFIER&&subtokens[2].value!="("&&subtokens[2].value!="=")
    // {
    //     std::string className=subtokens[0].value;
    //     std::string objectName=subtokens[1].value;
    //     std::cout<<"new object "<<objectName<<std::endl;
    //     new Object(className,objectName,env);
    //     std::cout<<"new object "<<objectName<<" success"<<std::endl;
    // }
    // else if(subtokens[0].type==IDENTIFIER&&subtokens[1].type==IDENTIFIER&&subtokens[2].value=="(")
    // {
    //     //构造函数，创建对象的同时调用构造函数
    //     std::string className=subtokens[0].value;
    //     std::string objectName=subtokens[1].value;
    //     std::cout<<"new object "<<objectName<<std::endl;
    //     Object*thisObject=new Object(className,objectName,env);
    //     std::unordered_map<std::string,ClassFunction*> thisFunctionTable=thisObject->function_table;
    //     std::string functionName=className;
    //     // if(thisFunctionTable.find(functionName)==thisFunctionTable.end())
    //     // {
    //     //     std::cout<<"not found classfunction"<<functionName;
    //     //     exit(1);
    //     // }
    //     std::cout<<functionName<<" call begin"<<std::endl;
    //     // thisFunctionTable[functionName]->call(subtokens,this->env);
    //     thisObject->myConstructor->callInline(subtokens,this->env);

    //     //可以理解为现在将构造函数的函数体内联
    //     //目的时为了更早确定数据成员的类型
    //     //否则在其他成员函数中使用数据成员时，可能会出现null
    //     //注：generate重载为内联形式
    //     thisObject->myConstructor->generateInline();
        
    //     std::cout<<"new object "<<objectName<<" success"<<std::endl;
    // }
    // else if(subtokens[0].type==IDENTIFIER&&subtokens[1].type==IDENTIFIER&&subtokens[2].value=="=")
    // {
    //     //对等号左边的对象调用copy，传入等号右边的对象
    //     std::cout<<"copy begin"<<std::endl;
    //     for(auto&i:subtokens)
    //     {
    //         std::cout<<i.value<<" ";
    //     }
    //     object_table[subtokens[1].value]->copy(object_table[subtokens[3].value]);
    // }
    //对实例构造进行整合
    //////////////////
    //////////////////
    else if(subtokens[0].type==IDENTIFIER&&subtokens[1].type==IDENTIFIER&&class_table.find(subtokens[0].value)!=class_table.end())
    {
        std::string className=subtokens[0].value;
        std::string objectName=subtokens[1].value;
        std::cout<<"new object "<<objectName<<std::endl;
        Object*thisObject=new Object(className,objectName,env);
        
        if(subtokens[2].type==SYMBOL&&subtokens[2].value=="(")
        {
            std::unordered_map<std::string,ClassFunction*> thisFunctionTable=thisObject->function_table;
        std::string functionName=className;
        // if(thisFunctionTable.find(functionName)==thisFunctionTable.end())
        // {
        //     std::cout<<"not found classfunction"<<functionName;
        //     exit(1);
        // }
        std::cout<<functionName<<" call begin"<<std::endl;
        // thisFunctionTable[functionName]->call(subtokens,this->env);
        thisObject->myConstructor->call(subtokens,this->env);

        //可以理解为现在将构造函数的函数体内联
        //目的时为了更早确定数据成员的类型
        //否则在其他成员函数中使用数据成员时，可能会出现null
        //注：generate重载为内联形式
        // thisObject->myConstructor->generateInline();
        }
        else if(subtokens[2].value=="=")
        {
            //对等号左边的对象调用copy，传入等号右边的对象
            std::cout<<"copy begin"<<std::endl;
            for(auto&i:subtokens)
            {
                std::cout<<i.value<<" ";
            }
            object_table[subtokens[1].value]->copy(object_table[subtokens[3].value]);
        }
        std::cout<<"new object "<<objectName<<" success"<<std::endl;
    }
    else if(subtokens[0].type==IDENTIFIER&&subtokens[1].type==SYMBOL&&subtokens[1].value=="->")
    {
        std::cout<<"in the call"<<std::endl;
        Object* thisObject=object_table[subtokens[0].value];
        std::unordered_map<std::string,ClassFunction*> thisFunctionTable=thisObject->function_table;
        std::string functionName=subtokens[2].value;


        if(thisFunctionTable.find(functionName)==thisFunctionTable.end())
        {
            std::cout<<"not found classfunction"<<functionName;
            exit(1);
        }
        std::cout<<functionName<<" call begin"<<std::endl;
        thisFunctionTable[functionName]->call(subtokens,this->env);
    }
    else if(subtokens[0].type==KEYWORD&&subtokens[0].value=="import")
    {
        std::cout << "import" << std::endl;

        //for(int i=1;i<subtokens.size();i++)
        //{
            std::cout<<subtokens[1].value<<" ";
            std::string fileName=subtokens[1].value;

            IO* input = new IO(fileName+".whup");
            
            std::string expression = input->read();

            delete input;

            checkBrackets::checkPar(expression,fileName+".whup");
            checkBrackets::checkBracket(expression,fileName+".whup");
            checkBrackets::checkBrace(expression,fileName + ".whup");
            printErrors();

            Lexer lexer(expression,fileName + ".whup");
            std::vector<Token> tokens = lexer.tokenize();
            //tokens.pop_back(); // 删除最后一个换行符

            // 使用得到的token集合进行语法分析，生成一个中间表示
            Block* block = new Block(tokens,env);

            namespace_table[fileName] = block->getEnv();

        //}
    } 
    else
    {
        pushErrors(subtokens[0],"unexpected token "+subtokens[0].value);
    }
}
