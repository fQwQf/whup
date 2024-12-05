#ifndef ASSIGN_H_
#define ASSIGN_H_

#ifndef EXPRESSION_H_
#include"expression.h"
#endif

class Assign
{
	Token var;//等号左侧变量
	Expr* expr;//等号右侧表达式

	Environment* env;//当前环境
	Environment* left_env;//等号左侧变量所在环境
public:
	Assign(std::vector<Token> code, Environment* env);
	void assign();
	void setEnv(Environment* env);
	std::string return_type();
};

#endif