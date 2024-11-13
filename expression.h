#ifndef EXPRESSION_H_
#define EXPRESSION_H_
#include <stack>
#include <vector>
#include <string>
#include"lexer.h"

class Expr
{
private:
	std::vector<Token> E_expr;
	std::stack<Token> E_ops;
	std::stack<Token> E_val;
	std::vector<Token> E_postfix;
	std::stack<Token> E_tac;

	double exprValue;
	int precedence(char op);			 //
	std::vector<Token> inFixToPostFix(); // 将中缀转为后缀
	void postFixToTAC();				 // 将后缀转为三地址码并存入三地址码栈中
public:
	Expr(const std::vector<Token> &expr) : E_expr(expr) {} // 用表达式词法单元串初始化

	void expr(std::stack<Token> &); // 合并对expr的所有处理,将得到的三地址码栈压入总栈？
};
#endif // !EXPRESSION_H_
