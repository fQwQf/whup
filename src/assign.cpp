#include"assign.h"

extern std::vector<ThreeAddressCode> tacs;

Assign::Assign(std::vector<Token> code, Environment* env)
{
	this->setEnv(env);
	var = code[0];
	code.erase(code.begin());
	code.erase(code.begin()); // 两次处理将等号和等号左侧变量消去
	expr = new Expr(code,env);

	//检查左边是否声明，未声明则自动声明
	if(env->get_var(var.value)=="null"){
		env->insert_var(var.value);
		env->change_type_var(var.value,expr->return_type());
	}
	assign();
}


void Assign::assign()
{
	tacs.push_back({ "","",expr->getTacResult(),var.value});
}

void Assign::setEnv(Environment* env)
{
	this->env = env;
}

std::string Assign::return_type(){
	expr->return_type();
}