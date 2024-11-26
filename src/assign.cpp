#include"assign.h"

extern std::vector<ThreeAddressCode> tacs;

Assign::Assign(std::vector<Token> code, Environment* env)
{
	this->setEnv(env);
	var = code[0];
	code.erase(code.begin());
	code.erase(code.begin()); // 两次处理将等号和等号左侧变量消去
	std::cout<<"right expr"<<std::endl;
	expr = new Expr(code,env);
	std::cout<<"expr tac.result"<<expr->getTacResult()<<std::endl;

	//检查左边是否声明，未声明则自动声明
	if(env->get_var(var.value)=="null"){
		env->insert_var(var.value);
		env->change_type_var(var.value,expr->return_type());

		std::cout << "var " << var.value << " declared automatically" << std::endl;
		std::cout << "var " << var.value << " type is " << expr->return_type() << std::endl;
		
	}else{
		std::cout<<"var "<<var.value<<" already declared"<<std::endl;

		std::cout<<expr->getTacResult()<<" type is "<<expr->return_type();
		env->change_type_var(var.value,expr->return_type());//导致类型问题！！！

		std::cout<<"var "<<var.value<<" type changed to "<<expr->return_type()<<std::endl;
	}
	assign();
}


void Assign::assign()
{
	tacs.push_back({ "","",expr->getTacResult(),env->get_var(var.value)});
}

void Assign::setEnv(Environment* env)
{
	this->env = env;
}

std::string Assign::return_type(){
	return expr->return_type();
}