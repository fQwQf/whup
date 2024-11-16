#include "var.h"
#include "assign.h"


void Var::declare()
{
    env->insert_var(name);
}

Var::Var(std::vector<Token> tokens,Environment* env)
{
    this->env = env;
    if (tokens.size() == 2){
        var(env, tokens[1].value);
    }else{
        var(env, "int", tokens[1].value);
        Assign(std::vector<Token>(tokens.begin()+1, tokens.end()),env);
    }
}

//分别是有类型和没有类型的变量声明
void Var::var(Environment *env, std::string type, std::string name)
{
    this->env = env;
    this->type = type;
    this->name = name;
    declare();
    env->change_type_var(name, type);
}

void Var::var(Environment *env, std::string name)
{
    this->env = env;
    this->name = name;
    declare();
}