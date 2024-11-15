#ifndef EXPRESSION_H_
#define EXPRESSION_H_
#include <stack>
#include <vector>
#include <string>
#include"parser.h"


std::string newTempVar();


class Assign;
class Expr {
private:
    std::vector<Token> E_expr;
    std::vector<Token> E_postfix;
    std::stack<Token> E_tac;
    ThreeAddressCode tac; // 三地址码输出

    Expr* left;
    Expr* right;

    double exprValue;
    int precedence(char op);			 //

    void matchPar(int& i);//用来跳过括号内容
public:
    Expr(const std::vector<Token>& expr) : E_expr(expr) {
        tac.result = newTempVar();
    } // 用表达式词法单元串初始化

    void expr(); // 合并对expr的所有处理,将得到的三地址码栈压入总栈？
    friend class Assign;
};
#endif // !EXPRESSION_H_
