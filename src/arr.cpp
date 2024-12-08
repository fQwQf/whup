#include "arr.h"
#include "assign.h"

extern std::vector<ThreeAddressCode> tacs;

void Arr::declare()
{
    for(int i = 0; i < size; i++)
    {
        env->insert_arr(name + "[" + std::to_string(i) + "]");
    }
}

Arr::Arr(std::vector<Token> tokens,Environment* env)
{
    this->env = env;
    if(tokens.size() == 4){
        //说明未传入初始数组大小，默认为 1
        arr(env, tokens[1].value, 1);
    }
    else if (tokens.size() == 5){
        arr(env, tokens[1].value, std::stoi(tokens[3].value));
    }else{
        //先判断返回类型
        arr(env, tokens[1].value,std::stoi(tokens[3].value));
        // Assign *ass = new Assign(std::vector<Token>(tokens.begin()+1, tokens.end()),env);
        initArr(std::vector<Token>(tokens.begin()+1, tokens.end()),env);
    }
}

void Arr::initArr(std::vector<Token> code,Environment* env)
{
    // int index=std::stoi(code[2`].value);

    //记录该变量
    Token var = code[0];
	//消去变量、中括号和等号
	code.erase(code.begin(),code.begin()+5);
    //去除表达式首尾大括号
    code.erase(code.begin());
    code.erase(code.end()-1);

    if(code.size() == 1)
    {
        //说明是单个值对数组初始化
        for(int i=0;i<size;i++)
        {
            Expr* expr = new Expr(code,env);
            tacs.push_back({ "=",expr->getTacResult(),"",env->get_arr(var.value+"["+std::to_string(i)+"]")});
        }
    }
}

void Arr::arr(Environment *env, std::string name, int size)
{
    this->env = env;
    this->name = name;
    this->size = size;
    declare();
}