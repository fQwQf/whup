#include"expression.h"
class Assign
{
	Token var;//�Ⱥ�������
	Expr* expr;//�Ⱥ��Ҳ���ʽ
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
	expr->E_tac.push({ "","",expr->tac.result,var.value });
}