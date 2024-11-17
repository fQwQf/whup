#include"if.h"
/*呃呃写完发现好像不用block，expr数组，在new临时变量IBT和IET时就已经把tac压入搭配tacs中了
不过作为一种记录过程的方式，也许可以利于拓展（？）*/
void If::martchIfElse(int&i,std::vector<Token>code)
{
	if (code[i].type == KEYWORD && code[i].value == "if")
	{
		i++;
		return;
	}
	else if (code[i].type == KEYWORD && code[i].value == "else")
	{
		i++;
		if (code[i].type == KEYWORD && code[i].value == "if")
		{
			i++;
			return;
		}
		else if (code[i].value == "{")
			return;
	}
}
void If::martchPar(int& i, std::vector<Token>code)
{
	if (code[i].value == "(")
	{
		int leftPar = 1;
		int rightPar = 0; // 分别记录已经读取的左括号右括号的个数,当相等时即可结束
		while (leftPar != rightPar)
		{
			--i;
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
void If::martchBrace(int& i, std::vector<Token>code)
{
	if (code[i].value == "{")
	{
		int leftBrace = 1;
		int rightBrace = 0; // 分别记录已经读取的左括号右括号的个数,当相等时即可结束
		while (leftBrace != rightBrace)
		{
			--i;
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

If::If(std::vector<Token> code, Environment* env) :If_Env(env)
{
	//临时变量用来在扫描过程中把code分别储存
	std::vector<std::vector<Token>>If_Expr_temps;
	std::vector<std::vector<Token>>If_Block_temps;
	std::vector<std::string>If_Block_labelArr;
	int pos = 0;
	while (pos < code.size())//做一次扫描的同时将code分别储存到各个expr和block中（以token形式）后续在进行tacs操作
	{
		//处理if和else的token
		martchIfElse(pos, code);

		//记录一个expr
		int Expr_begin = pos;
		martchPar(pos, code);
		If_Expr_temps.push_back(std::vector<Token>(code.begin() + Expr_begin+1, code.begin() + pos-1));
		pos++;
		//记录一个block
		int Block_begin = pos;
		martchBrace(pos, code);
		If_Block_temps.push_back(std::vector<Token>(code.begin() + Block_begin + 1, code.begin() + pos - 1));
		If_Block_labelArr.push_back(newTempLabel());//记录block代码的同时创建一个label，使得二者按索引对应
		pos++;
	}

	//下面先将expr处理，用于生成跳转语句
	for (int i = 0; i < If_Expr_temps.size(); i++)
	{
		Expr* IET = new Expr(If_Expr_temps[i], this->If_Env);
		If_Expr.push_back(IET);
		tacs.push_back({ "if_goto",IET->getTacResult(),"", If_Block_labelArr[i]});//生成跳转语句
	}
	/*与原定形式不太一样但效果相同，原定形式中没有对ti的计算，但只要计算步骤紧跟在跳转语句之前就没有影响*/

	//下面处理block
	for (int i = 0; i < If_Block_temps.size(); i++)
	{
		Block* IBT = new Block(If_Block_temps[i], If_Env);
		tacs.push_back({ "label","","",If_Block_labelArr[i] });//先给label
		If_Block.push_back(IBT);//再给代码
		tacs.push_back({ "goto","","",If_endLabel });//运行完前面的代码直接goto endLabel
	}

}