#include"assign.h"

extern std::vector<ThreeAddressCode> tacs;

Assign::Assign(std::vector<Token> code)
{
	var = code[0];
	code.erase(code.begin());
	code.erase(code.begin()); // 两次处理将等号和等号左侧变量消去
	expr = new Expr(code);
}

void assign();

void Assign::assign()
{
	expr->setEnv(env);
	expr->expr();
	tacs.push_back({ "","",expr->getTacResult(),var.value});
}

void Assign::setEnv(Environment* env)
{
	this->env = env;
}