#include"expression.h"
#include"block.cpp"
extern std::vector<ThreeAddressCode> tacs;
class While
{
	Expr* W_expr;
	Block* W_block;
public:
	While(std::vector<Token>& code);
	void whup_while ();
};

While::While(std::vector<Token>& code)
{
	code.pop_back();
	code.erase(code.begin());//Ԥ�Ƚ�while�ͣ��ʷ���Ԫȥ��

	//��ʼ��W_expr;
	std::vector<Token> temp_W_expr;
	int pos = 1;
	while (code[pos].value != ")")
	{
		temp_W_expr.push_back(code[pos]);
		pos++;
	}
	W_expr = new Expr(temp_W_expr);
	////��ʼ��W_block;
	//std::vector<Token> temp_W_block;
	//pos++;
	//while (code[pos].value != "}")
	//{
	//	temp_W_block.push_back(code[pos]);
	//	pos++;
	//}
}
void While::whup_while ()
{
	W_expr->expr();

}