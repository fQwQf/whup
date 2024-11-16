#include "expression.h"

extern std::vector<ThreeAddressCode> tacs;
class While
{
	Expr* While_Expr;
	Block* While_Block;

	Environment* While_env;
public:
	While(std::vector<Token>& code, Environment* env);
};