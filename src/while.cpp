#include"block.h"
#include"while.h"
void While::matchPar(int& i, std::vector<Token>code)
{
	if (code[i].value == "(")
	{
		int leftPar = 1;
		int rightPar = 0; // 分别记录已经读取的左括号右括号的个数,当相等时即可结束
		while (leftPar != rightPar)
		{
			++i;
			if (code[i].value == ")")
			{
				rightPar++;
			}
			else if (code[i].value == "(")
			{
				leftPar++;
			}
			else
				continue;
		}
	}
}
void While::matchBrace(int& i, std::vector<Token>code)
{
	if (code[i].value == "{")
	{
		int leftBrace = 1;
		int rightBrace = 0; // 分别记录已经读取的左括号右括号的个数,当相等时即可结束
		while (leftBrace != rightBrace)
		{
			++i;
			if (code[i].value == "}")
			{
				rightBrace++;
			}
			else if (code[i].value == "{")
			{
				leftBrace++;
			}
			else
				continue;
		}
	}
}
While::While(std::vector<Token>& code,Environment*env):While_env(env)
{
	std::cout<<"code lenths="<<code.size()<<std::endl;
	code.erase(code.begin());//预先将while和；词法单元去掉
	//首先设计label用于后续跳转
	std::string While_Expr_label = newTempLabel();
	std::string While_Block_label = newTempLabel();
	std::string While_end = newTempLabel();
	//先为expression提供标签
	tacs.push_back({ "label","","",While_Expr_label});//一个特殊的“三地址”：“label：”
	//初始化W_expr;
	int pos = 0;
	matchPar(pos, code);//跳过括号地同时得到）的位置从而初始化expr
	if (pos <= 1 || pos > code.size()) 
	{
    throw std::length_error("Invalid position for creating vector");
	}
	std::cout<<"pos="<<pos<<std::endl;
	While_Expr = new Expr(std::vector<Token>(code.begin(), code.begin() + pos+1),this->While_env);
	/*
	唐得没边之：搞错迭代器开始和结束的范围
	*/
	//注意，此时已经将三地址码压入tacs中
	std::cout<<"expr done"<<std::endl;
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

	std::cout<<"Block begin"<<std::endl;
	//初始化W_block;
	++pos;//跳过“）”
	std::cout<<code.size()<<std::endl;
	int Block_begin = pos;
	std::cout<<"pos="<<pos<<std::endl;
	matchBrace(pos, code);
	std::cout<<"pos="<<pos<<std::endl;
	While_Block = new Block(std::vector<Token>(code.begin() + Block_begin, code.begin() + pos+1), While_env);

	std::cout<<"xunhuan begin."<<std::endl;
	//跳转再次进入while
	tacs.push_back({ "goto","","",While_Expr_label });

	//最后为结束标签
	tacs.push_back({ "label","","",While_end });
}