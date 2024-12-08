#include "return.h"
#include "expression.h"

extern std::string function_ret_label;     // 函数返回标签，可用于检测是否在处理函数。
extern std::string function_return_value;
extern std::string function_return_type;

extern std::vector<ThreeAddressCode> tacs; // 存储三地址代码的向量

Return::Return(Environment *env)
{
    tacs.push_back({RET,"return", "", "", ""});
}

Return::Return(std::vector<Token> tokens, Environment *env)
{
    this->env = env;
    tokens.erase(tokens.begin()); // 删除 "return" 关键字
    if(tokens.size() == 1)
    {
        if (tokens[0].type == IDENTIFIER)
        {
            if (tokens[0].processed)
            {
                if (function_return_type == "string")
                {
                    tacs.push_back({STRASSIGN, "=", tokens[0].value, "", function_return_value});
                }
                else
                {
                    tacs.push_back({ASSIGN, "=", tokens[0].value, "", function_return_value});
                }
            }
            else
            {
                if (env->get_type_var(tokens[0].value) == "string")
                {
                    tacs.push_back({STRASSIGN, "=", env->get_var(tokens[0].value), "", function_return_value});
                }
                else
                {
                    tacs.push_back({ASSIGN, "=", env->get_var(tokens[0].value), "", function_return_value});
                }
            }
        }
        else
        {
            if(tokens[0].type == STRING)
            {
                tacs.push_back({STRASSIGN,"=", tokens[0].value, "", function_return_value});
            }
            else
            {
                tacs.push_back({ASSIGN,"=", tokens[0].value, "", function_return_value});
            }
        }
    }
    else{
        Expr* expr = new Expr(tokens, env);
        if(expr->return_type() == "string")
        {
            tacs.push_back({STRASSIGN,"=", expr->getTacResult(), "", function_return_value});
        }
        else
        tacs.push_back({ASSIGN,"=", expr->getTacResult(), "", function_return_value});
    }
    
    tacs.push_back({RET,"return", "", "", ""}); // 生成返回语句
}
