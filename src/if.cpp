#include"if.h"
bool If::martchIfElse(int&i,std::vector<Token>code)
{
	if (code[i].type == KEYWORD && code[i].value == "if")
	{
		i++;
		return 1;
	}
	else if (code[i].type == KEYWORD && code[i].value == "else")
	{
		i++;
		if (code[i].type == KEYWORD && code[i].value == "if")
		{
			i++;
			return 1;
		}
		else if (code[i].value == "{")
			return 0;
	}
}

If::If(std::vector<Token> code, Environment* env) :If_Env(env)
{
	//code.erase(code.begin());//预先将if删去

	////设计label用于跳转
	//std::string If_Block_label = newTempLabel();
	//std::string nextIf = newTempLabel();
	////初始化W_expr;
	//std::vector<Token> temp_If_Expr;
	//int pos = 1;//不录入括号
	//while (code[pos].value != ")")
	//{
	//	temp_If_Expr.push_back(code[pos]);
	//	pos++;
	//}
	//If_Expr = new Expr(temp_If_Expr, env);
	////注意，此时已经将三地址码压入tacs中

	////处理判断语句;
	//tacs.push_back({ "if_goto",If_Expr->getTacResult(),"",If_Block_label });//true则进入block
	//tacs.push_back({ "goto","","",nextIf});

	////初始化W_block;
	//std::vector<Token> temp_If_Block;
	//pos += 2;//跳过）和{
	//while (code[pos].value != "}")
	//{
	//	temp_If_Block.push_back(code[pos]);
	//	pos++;
	//}
	//If_Block = new Block(temp_If_Block, env);
	////注意此时已经将三地址码压入tacs中

	//临时变量用来在扫描过程中把code分别储存
	std::vector<std::vector<Token>>If_Expr_temps;
	std::vector<std::vector<Token>>If_Block_temps;
	
	int pos = 0;
	while (pos < code.size())
	{
		//判断接下来的结构
		bool isDone=martchIfElse(pos, code);

		//记录一个expr
		int Expr_begin = pos;
		martchPar(pos, code);
		If_Expr_temps.push_back(std::vector<Token>(code.begin() + Expr_begin, code.begin() + pos));

		//记录一个block
	}
	
}