#include "arr.h"
#include "assign.h"
#include "WHUPstream.h"

extern WHUPstream_compile1 WHUPout;

extern std::vector<ThreeAddressCode> tacs;

std::vector<Arr> arrs;//记录所有数组
extern std::vector<ThreeAddressCode> tacArrs;

void Arr::declare()
{
    env->insert_arr(name);
    if(type=="number")
        tacArrs.push_back({ARRSET, std::to_string(size),"number",env->get_arr(name)});
    else if(type=="string")
        tacArrs.push_back({ARRSET, std::to_string(size),"string",env->get_arr(name)});
    WHUPout<<"arr declear success!!   tacArrs.size ="<<tacArrs.size()<<std::endl;
}

Arr::Arr(std::vector<Token> tokens,Environment* env,std::string type):type(type)
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
            // WHUPout<<"dimension = "<<dimension<<std::endl;
            std::vector<Token> temp_tokens=tokens;
            //消去初始化量、中括号和等号
            for(int i=0;i<4;i++) 
                temp_tokens.pop_back();
            arr(env, tokens[1].value,temp_tokens,dimension,1);

            WHUPout<<"dimension = "<<this->dimension<<std::endl;
            WHUPout<<"size = "<<this->size<<std::endl;
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
    // for(auto i : arrs)
    // {
    //     WHUPout<<"name = "<<i.name<<std::endl;
    //     WHUPout<<"dimension = "<<i.dimension<<std::endl;
    //     WHUPout<<"size = "<<i.size<<std::endl;
    // }
}

void Arr::initArr(std::vector<Token> code,Environment* env)
{
    // int index=std::stoi(code[2].value);

    //记录该变量
    Token var = code[0];
	//消去变量、中括号和等号,只留下等号右边
	code.erase(code.begin(),code.end()-3);
    //去除表达式首尾大括号
    code.erase(code.begin());
    code.erase(code.end()-1);

    if(code.size() == 1)
    {
        //说明是单个值对数组初始化
        Expr* expr = new Expr(code,env);
        for(int i=0;i<size;i++)
        {
            std::vector<Token> temptoken;
            Token index={NUMBER,std::to_string(i),code[0].line_number,code[0].file_name};
            temptoken.push_back(index);
            Expr* index_expr = new Expr(temptoken,env);
            tacs.push_back({ASSIGN,"=",expr->getTacResult(),"",env->get_arr(var.value)+">->"+index_expr->getTacResult()});
            WHUPout<<"init arr success!! "<<std::endl;
        }
    }
}

void Arr::arr(Environment *env, std::string name, std::vector<Token> tokens ,int dimension,int size)
{
    // for(auto i : tokens)
    // {
    //     WHUPout<<i.value<<" ";
    // }
    tokens.pop_back();
    // if(std::isdigit(tokens.back().value))
    if(!len.empty())
        this->len.push_back(this->len.back()*std::stoi(tokens.back().value));
    else
        this->len.push_back(std::stoi(tokens.back().value));
    // WHUPout<<"len = "<<this->len.back()<<std::endl;
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
    // WHUPout<<"dimension = "<<this->dimension<<std::endl;
}