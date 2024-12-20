#include"block.h"
#include"while.h"
#include "WHUPstream.h"

extern WHUPstream_compile1 WHUPout;
extern int global_env_id;  //全局EnvironmentID计数器
std::stack<int> global_circulation_id;
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
While::While(std::vector<Token> &code,Environment*env):While_env(env)
{
	WHUPout<<"code lenths="<<code.size()<<std::endl;
	code.erase(code.begin());//预先将while和；词法单元去掉
	//首先设计label用于后续跳转
	std::string While_Block_label = newTempLabel();

	//需要设计这里的变量名
	global_circulation_id.push(global_env_id);//这里的id是为了区分不同的循环
	//
	std::string beginLabelToEnv="Circulation_beginlabel_"+std::to_string(global_circulation_id.top());
	While_env->insert_var(beginLabelToEnv);
	std::string While_begin_label = While_env->get_var(beginLabelToEnv);
	//
	std::string endLabelToEnv="Circulation_endlabel_"+std::to_string(global_circulation_id.top());
	While_env->insert_var(endLabelToEnv);
	std::string While_end_label = While_env->get_var(endLabelToEnv);
	//这里把一个标识符插入符号表，该标识符指向

	//先为expression提供标签
	tacs.push_back({LABEL, "label","","",While_begin_label});//一个特殊的“三地址”：“label：”
	//初始化W_expr;
	int pos = 0;
	matchPar(pos, code);//跳过括号地同时得到）的位置从而初始化expr
	if (pos <= 1 || pos > code.size())
	{
    throw std::length_error("Invalid position for creating vector");
	}
	WHUPout<<"pos="<<pos<<std::endl;
	While_Expr = new Expr(std::vector<Token>(code.begin(), code.begin() + pos+1),this->While_env);
	/*
	唐得没边之：搞错迭代器开始和结束的范围
	*/
	//注意，此时已经将三地址码压入tacs中
	WHUPout<<"expr done"<<std::endl;
	//先处理判断语句
	std::string While_Expr_result = While_Expr->getTacResult();
	
	ThreeAddressCode While_Expr_tac;
	While_Expr_tac.opperator=IF_GOTO;
	While_Expr_tac.op = "if_goto";
	While_Expr_tac.arg1 = While_Expr_result;
	While_Expr_tac.result = While_Block_label;
	tacs.push_back(While_Expr_tac);//如果true进入block

	tacs.push_back({GOTO, "goto","","",While_end_label });//如果false跳转至结束

	//为block提供标签
	tacs.push_back({LABEL, "label","","",While_Block_label });

	WHUPout<<"Block begin"<<std::endl;
	//初始化W_block;
	++pos;//跳过“）”
	WHUPout<<code.size()<<std::endl;
	int Block_begin = pos;
	WHUPout<<"pos="<<pos<<std::endl;
	matchBrace(pos, code);
	WHUPout<<"pos="<<pos<<std::endl;
	While_Block = new Block(std::vector<Token>(code.begin() + Block_begin + 1, code.begin() + pos+1), While_env);

	WHUPout<<"xunhuan begin."<<std::endl;
	//跳转再次进入while
	tacs.push_back({GOTO, "goto","","",While_begin_label });

	//最后为结束标签
	tacs.push_back({LABEL, "label","","",While_end_label });
}