#include"expression.h"
extern std::vector<ThreeAddressCode> tacs;
class Assign
{
	Token var;//等号左侧变量
	Expr* expr;//等号右侧表达式
	ThreeAddressCode tac;
public:
	Assign(std::vector<Token> code)
	{
		var = code[0];
		code.erase(code.begin());
		code.erase(code.begin());
		expr = new Expr(code);
	}
	void assign();
};

void Assign::assign()
{
	expr->expr();
	tacs.push_back({ "","",expr->tac.result,var.value });
}