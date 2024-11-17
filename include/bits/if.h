#include"whupc.h"
#include"block.h"
extern std::vector<ThreeAddressCode>tacs;
class If
{
	Expr* If_Expr;
	Block* If_Block;
	Environment* If_Env;
public:
	If(std::vector<Token>, Environment*);
};