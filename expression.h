#ifndef EXPRESSION_H_
#define EXPRESSION_H_
#include <stack>
#include <vector>
#include <string>
#include"lexer.h"


std::string newTempVar();

/*
*结构：三地址码
*
*储存四个字符串，分别代表运算符，操作数1，操作数2，结果
*/
struct ThreeAddressCode {
    std::string op;   // 操作符
    std::string arg1;
    std::string arg2;
    std::string result;
};

class Expr {
    private:
        std::vector<Token> E_expr;
        std::stack<Token> E_ops;
        std::stack<Token> E_val;
        std::vector<Token> E_postfix;
        std::stack<Token> E_tac;
        ThreeAddressCode tac; // 三地址码输出

        Expr* left;
        Expr* right;

        double exprValue;
        int precedence(char op);			 //
        std::vector<Token> inFixToPostFix(); // 将中缀转为后缀
        void postFixToTAC();				 // 将后缀转为三地址码并存入三地址码栈中
    public:
        Expr(const std::vector<Token> &expr) : E_expr(expr) {
            tac.result = newTempVar();
        } // 用表达式词法单元串初始化

        void expr(); // 合并对expr的所有处理,将得到的三地址码栈压入总栈？
};
#endif // !EXPRESSION_H_
