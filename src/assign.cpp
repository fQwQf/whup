#include"expression.h"
extern std::vector<ThreeAddressCode> tacs;
class Assign
{
	Token var;//等号左侧变量
	Expr* expr;//等号右侧表达式
public:
	Assign(std::vector<Token> code)
	{
		var = code[0];
		code.erase(code.begin());
		code.erase(code.begin());//两次处理将等号和等号左侧变量消去
		expr = new Expr(code);
	}
	void assign();
};

void Assign::assign()
{
	expr->expr();
	tacs.push_back({ "","",expr->getTacResult(),var.value});
}
