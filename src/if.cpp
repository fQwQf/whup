#include"if.h"
/*呃呃写完发现好像不用block，expr数组，在new临时变量IBT和IET时就已经把tac压入搭配tacs中了
不过作为一种记录过程的方式，也许可以利于拓展（？）*/
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

		std::cout<<"else judge success"<<std::endl;

		if (code[i].type == KEYWORD && code[i].value == "if")
		{
			i++;
			return 1;
		}
		else if (code[i].value == "{")
			return 0;
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
void If::martchBrace(int& i, std::vector<Token>code)
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

If::If(std::vector<Token> code, Environment* env) :If_Env(env)
{
	//临时变量用来在扫描过程中把code分别储存
	std::vector<std::vector<Token>>If_Expr_temps;
	std::vector<std::vector<Token>>If_Block_temps;
	//用一个label数组来记录生成了多少个Block并接收对应的label
	std::vector<std::string>If_Block_labelArr;
	std::string If_endLabel=newTempLabel();
	int pos = 0;

	std::cout<<"begin"<<std::endl;

	while (pos < code.size())//做一次扫描的同时将code分别储存到各个expr和block中（以token形式）后续在进行tacs操作
	{
		//处理if和else的token
		bool isIfElse=martchIfElse(pos, code);

		//记录一个expr
		if(isIfElse)
		{
			int Expr_begin = pos;
			martchPar(pos, code);
			If_Expr_temps.push_back(std::vector<Token>(code.begin() + Expr_begin+1, code.begin() + pos));
			pos++;
		}
		else
		{
			std::cout<<"no expr"<<std::endl;
		}
		

		std::cout<<"pos="<<pos<<std::endl;
		std::cout<<"expr record success"<<std::endl;

		//记录一个block
		int Block_begin = pos;
		martchBrace(pos, code);
		If_Block_temps.push_back(std::vector<Token>(code.begin() + Block_begin + 1, code.begin() + pos ));
		If_Block_labelArr.push_back(newTempLabel());//记录block代码的同时创建一个label，使得二者按索引对应
		pos++;

		std::cout<<"pos="<<pos<<std::endl;
		std::cout<<"block record success"<<std::endl;
	}

	std::cout<<"ief.size()="<<If_Expr_temps.size()<<std::endl;
	std::cout<<"ifb.size()="<<If_Block_temps.size()<<std::endl;

	//下面先将expr处理，用于生成跳转语句
	for (int i = 0; i < If_Expr_temps.size(); i++)
	{
		Expr* IET = new Expr(If_Expr_temps[i], this->If_Env);
		If_Expr.push_back(IET);
		tacs.push_back({ "if_goto",IET->getTacResult(),"", If_Block_labelArr[i]});//生成跳转语句
	}

	std::cout<<"expr generate success"<<std::endl;

	if (If_Expr_temps.size() < If_Block_temps.size())//可能结尾为else{}，不会生成Expr
	{
		std::cout<<"defualt goto generate"<<std::endl;

		tacs.push_back({ "goto","","",*(If_Block_labelArr.end()-1) });//goto用于实现默认进入else后的block

		std::cout<<"defualt goto generate success"<<std::endl;
	}
	/*
	与原定形式不太一样但效果相同
		1.原定形式中没有对ti的计算，但只要计算步骤紧跟在跳转语句之前就没有影响
		2.修改了对else{}的处理,使得else{block}的block与其他部分处理的区别仅在于if goto跳转语句上，用goto代替if goto来实现"默认"
		  其他的生成形式完全相同.
		  (在示例代码中可见)
	*/

	//下面处理block
	for (int i = 0; i < If_Block_temps.size(); i++)
	{
		tacs.push_back({ "label","","",If_Block_labelArr[i] });//先给label
		Block* IBT = new Block(If_Block_temps[i], If_Env);
		If_Block.push_back(IBT);//再给代码
		tacs.push_back({ "goto","","",If_endLabel });//运行完前面的代码直接goto endLabel
	}

	//最后加上endlabel
	tacs.push_back({"label","","",If_endLabel});
}

/*
由于我是在默认code形式正确的前提下编写，下面记录一下可能的错误：
	1.（expression）内部为空，可能导致问题，因为对code本身处理时做了去头去尾的操作（不做也可以，但是直接传一个（exrp）在Expr类生成时不够优雅
	2.block初始化带不带“{”和“}”？我在处理时去掉了{}，（又去头去尾了。
	3.对于martchIfElse（），可以扩展它的所有可能来进行报错
注中注：while类也可能存在同样问题.
*/