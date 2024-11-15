#ifndef EXPRESSION_H_
#define EXPRESSION_H_
#include <bits/stdc++.h>

#ifndef PARSER_H_
#include"whup_parser.h"
#endif


std::string newTempVar();


class Expr {
private:
    std::vector<Token> E_expr;
    ThreeAddressCode tac; // 三地址码输出

    Expr* left;
    Expr* right;

    Environment* env;

    void matchPar(int& i);//用来跳过括号内容
public:
    Expr(const std::vector<Token>& expr,Environment* env);

    void expr(); // 合并对expr的所有处理,将得到的三地址码栈压入总栈
    std::string getTacResult()
    {
        return tac.result;
    }

    void setEnv(Environment* env);
};
#endif
