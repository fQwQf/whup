#ifndef ASSIGN_H_
#define ASSIGN_H_
#include"expression.h"

class Assign
{
	Token var;//等号左侧变量
	Expr* expr;//等号右侧表达式

    Environment* env;//当前环境
public:
	Assign(std::vector<Token> code);
	void assign();
    void setEnv(Environment* env);
};

#endif