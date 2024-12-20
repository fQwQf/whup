#include"whupc.h"
#include"block.h"
extern std::vector<ThreeAddressCode>tacs;
class If
{
	std::vector<Expr*>If_Expr;//可能有一系列ifelse，但是无所谓，一个expression控制一个block
	std::vector<Block*>If_Block;
	std::vector<Environment*> If_Block_Env;
	std::string If_endLabel;//各个block统一跳转至endLabel处
	Environment* If_Env;
public:
	If(std::vector<Token>, Environment*);
	//匹配小括号的函数
	void martchPar(int& i, std::vector<Token>code);
	//匹配大括号的函数
	void martchBrace(int& i, std::vector<Token>code);

	//匹配if else是否符合语法规则
	bool martchIfElse(int& i, std::vector<Token>code);//1表示继续，0表示else后不再有if
};