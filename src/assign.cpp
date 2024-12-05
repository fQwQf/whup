#include"assign.h"

extern std::vector<ThreeAddressCode> tacs;
extern std::unordered_map<std::string, Environment*> namespace_table; // 存储命名空间名和对应的Environment对象的哈希表


Assign::Assign(std::vector<Token> code, Environment* env)
{
	//TODO 这里检查是否有多个等号
	this->setEnv(env);

	if(code[1].value !="::"){
		var = code[0];
		code.erase(code.begin());
		code.erase(code.begin()); // 两次处理将等号和等号左侧变量消去
		left_env = env;
	}else{
	    left_env = namespace_table[code[0].value];
		var = code[2];
		code.erase(code.begin(),code.begin()+4);
	}
	
	std::cout << "right expr" << std::endl;
	expr = new Expr(code,env);
	std::cout << "expr tac.result" << expr->getTacResult() << std::endl;

	//检查左边是否声明，未声明则自动声明
	if(left_env->get_var(var.value)=="null"){
		left_env->insert_var(var.value);
		left_env->change_type_var(var.value,expr->return_type());

		std::cout << "var " << var.value << " declared automatically" << std::endl;
		std::cout << "var " << var.value << " type is " << expr->return_type() << std::endl;
		
	}else{
		std::cout << "var " << var.value << " already declared" << std::endl;

		std::cout << expr->getTacResult() << " type is " << expr->return_type();
		left_env->change_type_var(var.value,expr->return_type());//导致类型问题！！！

		std::cout << "var " <<var.value << " type changed to " << expr->return_type() << std::endl;
	}
	assign();
}


void Assign::assign()
{
	if(expr->return_type()=="string")
		tacs.push_back({STRASSIGN, "=",expr->getTacResult(),"",left_env->get_var(var.value)});
	else
	tacs.push_back({ASSIGN, "=",expr->getTacResult(),"",left_env->get_var(var.value)});
}

void Assign::setEnv(Environment* env)
{
	this->env = env;
}

std::string Assign::return_type(){
	return expr->return_type();
}