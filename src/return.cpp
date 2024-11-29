#include "return.h"
#include "expression.h"

extern std::string function_ret_label;     // 函数返回标签，可用于检测是否在处理函数。
extern std::string function_return_value;

extern std::vector<ThreeAddressCode> tacs; // 存储三地址代码的向量

Return::Return(Environment *env)
{
    tacs.push_back({"if_goto", "true", "", function_ret_label}); // 如果当前在处理函数，则跳转到函数返回标签
}

Return::Return(std::vector<Token> tokens, Environment *env)
{
    this->env = env;
    tokens.erase(tokens.begin()); // 删除 "return" 关键字
    if(tokens.size() == 1){
        if (tokens[0].type == IDENTIFIER)
        {
            tacs.push_back({"=", env->get_var(tokens[0].value), "", function_return_value});
        }
        else
        {
            tacs.push_back({"=", tokens[0].value, "", function_return_value});
        }
    }else{
        Expr* expr = new Expr(tokens, env);
        tacs.push_back({"=", expr->getTacResult(), "", function_return_value});
    }
    
    tacs.push_back({"return", "", "", ""}); // 生成返回语句
}
