#include "print.h"

extern std::vector<ThreeAddressCode> tacs; // 存储三地址代码的向量

//接收一个含有Token的vector，作为expr的输入，输出该expr的结果
Print::Print(std::vector<Token> &tokens,Environment* env) {
    Expr* expr = new Expr(tokens,env);
    tacs.push_back({"print",expr->getTacResult(),"",""});

}