#include"block.h"
#include"while.h"

While::While(std::vector<Token>& code,Environment*env):While_env(env)
{
	
	code.erase(code.begin());//预先将while和；词法单元去掉
	//首先设计label用于后续跳转
	std::string While_Expr_label = newTempLabel();
	std::string While_Block_label = newTempLabel();
	std::string While_end = newTempLabel();

	//先为expression提供标签
	tacs.push_back({ "label","","",While_Expr_label});//一个特殊的“三地址”：“label：”

	//初始化W_expr;
	std::vector<Token> temp_While_Expr;
	int pos = 1;//不录入括号
	while (code[pos].value != ")")
	{
		temp_While_Expr.push_back(code[pos]);
		pos++;
	}
	While_Expr = new Expr(temp_While_Expr,env);
	//注意，此时已经将三地址码压入tacs中

	//先处理判断语句
	std::string While_Expr_result = While_Expr->getTacResult();
	ThreeAddressCode While_Expr_tac;
	While_Expr_tac.op = "if_goto";
	While_Expr_tac.arg1 = While_Expr_result;
	While_Expr_tac.result = While_Block_label;
	tacs.push_back(While_Expr_tac);//如果true进入block

	tacs.push_back({ "goto","","",While_end });//如果false跳转至结束

	//为block提供标签
	tacs.push_back({ "label","","",While_Block_label });
	//初始化W_block;
	std::vector<Token> temp_While_Block;
	pos+=2;//跳过）和{
	while (code[pos].value != "}")
	{
		temp_While_Block.push_back(code[pos]);
		pos++;
	}
	While_Block = new Block(temp_While_Block, env);
}