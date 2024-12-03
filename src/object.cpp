#include"object.h"
#include"var.h"
#include"classfunction.h"
#include"class.h"
extern std::unordered_map<std::string,Class*>class_table;
extern std::vector<ThreeAddressCode>tacs;

std::vector<std::unordered_map<std::string,ClassFunction*>> all_Object_function_table;  
//引入一个全局的向量，来储存所有对象的函数表指针，以便在函数调用时查找函数
std::unordered_map<std::string,Object*>object_table;
//引入一个全局的实例表，记录实例名与其Object的对应关系，主要是为了得到Object的函数表的信息

void Object::matchBrace(int &i,std::vector<Token> &tokens)
{
    if (tokens[i].value == "{")
    {
        int leftPar = 1;
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
//两个核心功能
void Object::var_declare(std::vector<Token>subtokens)
{
    // for(int i=1;i<subtokens.size();i++)//i=1是因为第一个是var
    // {
    //     if()
    //     if(subtokens[i].type==IDENTIFIER&&subtokens[i+1].value=="=")
    //     {
    //         subtokens.insert(subtokens.begin()+i+1,Token{SYMBOL,"=",subtokens[i].line_number});
    //         subtokens.insert(subtokens.begin()+i+2,Token{NUMBER,"0",subtokens[i].line_number});
    //     }
    // }
    new Var(subtokens,Object_env);
    //与Var类的构造函数相同，将变量声明插入实例的符号表
}
void Object::function_declare(std::vector<Token>subtokens)
{
    //与普通function的区别在于，将函数插入实例的函数表
    new ClassFunction(subtokens,Object_env,this->function_table);
}

void Object::constuctor_declare(std::vector<Token>subtokens)
{
    //构造函数的声明
    myConstructor=new ClassFunction(subtokens,Object_env,this->function_table);
}

void Object::copy(Object*ptrObject)
{
    std::cout<<this->Object_name<<std::endl;
    for(auto&i:this->Object_env->var_table)
    {
        for(auto&j:ptrObject->Object_env->var_table)
        {
            if(i.first==j.first)
            {
                ThreeAddressCode tac;
                this->Object_env->change_type_var(i.first,j.second);
                tac.op="=";
                tac.result=this->Object_env->get_var(i.first);
                tac.arg1=ptrObject->Object_env->get_var(j.first);
                tacs.push_back(tac);
            }
        }
    }
}






//generator函数，用来整合变量声明与函数声明
void Object::generator(std::vector<Token>subtokens)
{
    if(subtokens[0].type==KEYWORD&&subtokens[0].value=="var")
    {
        var_declare(subtokens);
        std::cout<<"var "<<subtokens[1].value<<" declare success"<<std::endl;
    }
    else if(subtokens[0].type==KEYWORD&&subtokens[0].value=="function")
    {
        function_declare(subtokens);
        std::cout<<"function "<<subtokens[1].value<<" declare success"<<std::endl;
    }
    else if(subtokens[0].type==IDENTIFIER&&subtokens[0].value==Class_name)
    {
        //构造函数
        constuctor_declare(subtokens);
        std::cout<<"constructor declare success"<<std::endl;
    }
    else
    {
        std::cout<<"error: unknown statement"<<std::endl;
    }
}

Object::Object(std::string className,std::string objectName,Environment*env)
{
    this->Object_env=new Environment(env->backToGlobal());//一方面创造新的符号表，另一方面用全局环境来做初始化
    object_table[objectName]=this;//将实例插入实例表
    this->Object_name=objectName;
    this->Class_name=className;
    std::vector<Token> tokens=class_table[className]->getStatements();//通过className，在类表中找到对应的声明语句
    //接下来按分号为间隔扫描得到独立语句

    std::cout<<"get statments!"<<std::endl;
    for(auto&i:tokens)
    {
        std::cout<<i.value<<" ";
    }
    std::cout<<std::endl;

    int last_semicolon = 0;

    for (int i = 0; i < tokens.size(); i++)
    {
        matchBrace(i, tokens);
        if (tokens[i].type == SYMBOL && tokens[i].value == ";")
        {
            std::vector<Token> subtokens(tokens.begin() + last_semicolon, tokens.begin() + i);
            last_semicolon = i+1;
            //得到了subtokens
            generator(subtokens);
        }
    }

    
    //将函数表插入全局函数表
    all_Object_function_table.push_back(this->function_table);
}

