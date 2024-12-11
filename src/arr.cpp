#include "arr.h"
#include "assign.h"

extern std::vector<ThreeAddressCode> tacs;

std::vector<Arr> arrs;//记录所有数组
extern std::vector<ThreeAddressCode> tacArrs;

void Arr::declare()
{
    env->insert_arr(name);
    if(type=="number")
        tacArrs.push_back({ARRSET,"", std::to_string(size),"number",env->get_arr(name)});
    else if(type=="string")
        tacArrs.push_back({ARRSET,"", std::to_string(size),"string",env->get_arr(name)});
}

Arr::Arr(std::vector<Token> tokens,Environment* env,std::string type)
{
    this->env = env;
    int dimension ;
    bool isAssign = false;
    for(auto i : tokens)
    {
        //判断是否有赋值语句
        if(i.value=="=")
        {
            isAssign = true;
            //先完成声明
            dimension = (tokens.size()-6)/3;
            std::cout<<"dimension = "<<dimension<<std::endl;
            std::vector<Token> temp_tokens=tokens;
            //消去初始化量、中括号和等号
            for(int i=0;i<4;i++) 
                temp_tokens.pop_back();
            arr(env, tokens[1].value,temp_tokens,dimension,1);

            std::cout<<"dimension = "<<this->dimension<<std::endl;
            std::cout<<"size = "<<this->size<<std::endl;
            // Assign *ass = new Assign(std::vector<Token>(tokens.begin()+1, tokens.end()),env);
            // tokens.erase(tokens.begin(),tokens.end()-4);
            initArr(std::vector<Token>(tokens.begin()+1, tokens.end()),env);
            break;
        }
    }
    if(!isAssign)
    {
        dimension = (tokens.size()-2)/3;
        arr(env, tokens[1].value, tokens,dimension,1);
    }

    //记录该数组
    arrs.push_back(*this);
}

void Arr::initArr(std::vector<Token> code,Environment* env)
{
    // int index=std::stoi(code[2].value);

    //记录该变量
    Token var = code[0];
	//消去变量、中括号和等号,只留下等号右边
	code.erase(code.begin(),code.end()-4);
    //去除表达式首尾大括号
    code.erase(code.begin());
    code.erase(code.end()-1);

    if(code.size() == 1)
    {
        //说明是单个值对数组初始化
        for(int i=0;i<size;i++)
        {
            Expr* expr = new Expr(code,env);
            tacArrs.push_back({ARRASSIGN,"=",std::to_string(i),expr->getTacResult(),env->get_arr(var.value)});
        }
    }
}

void Arr::arr(Environment *env, std::string name, std::vector<Token> tokens ,int dimension,int size)
{
    // for(auto i : tokens)
    // {
    //     std::cout<<i.value<<" ";
    // }
    tokens.pop_back();
    // if(std::isdigit(tokens.back().value))
    if(!len.empty())
        this->len.push_back(this->len.back()*std::stoi(tokens.back().value));
    else
        this->len.push_back(std::stoi(tokens.back().value));
    // std::cout<<"len = "<<this->len.back()<<std::endl;
    size*=std::stoi(tokens.back().value);
    tokens.pop_back();
    tokens.pop_back();
    this->dimension = dimension;
    if(dimension==1)
    {
        this->env = env;
        this->name = name;
        this->size = size;
        declare();
        return;
    }

    arr(env,name,tokens,dimension-1,size);

    this->dimension = dimension;

}