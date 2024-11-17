#include"whupc.h"
#include"block.h"
extern std::vector<ThreeAddressCode>tacs;
class If
{
	std::vector<Expr*>If_Expr;//可能有一系列ifelse，但是无所谓，一个expression控制一个block
	std::vector<Block*>If_Block;
	If* If_else;//采用嵌套方式，若有if后else部分则创建一个if的子节点

	Environment* If_Env;
public:
	If(std::vector<Token>, Environment*);
};