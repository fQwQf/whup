#include"assign.h"
#include"check.h"
#include"arr.h"
#include "WHUPstream.h"

extern std::vector<ThreeAddressCode> tacs;
extern std::vector<Arr> arrs;
extern std::vector<ThreeAddressCode> tacArrs; 
extern std::unordered_map<std::string, Environment*> namespace_table; // 存储命名空间名和对应的Environment对象的哈希表
extern std::unordered_map<std::string,std::string>var_declares;

extern WHUPstream_compile1 WHUPout;


void assign_matchBracket(std::vector<Token> E_expr,int &i)
{
    if (E_expr[i].value == "[")
    {
        int leftPar = 1;
        int rightPar = 0; // 分别记录已经读取的左括号右括号的个数,当相等时即可结束
        while (leftPar != rightPar)
        {
            i++;
            if (E_expr[i].value == "]")
            {
                rightPar++;
            }
            else if (E_expr[i].value == "[")
            {
                leftPar++;
            }
            else
                continue;
        }
    }
}

Assign::Assign(std::vector<Token> code, Environment* env)
{
	bool isArr = false;
	std::vector<Token> indextokens;
	int thisarr=0;
	if(code[1].value !="::"){
		this->setEnv(env);
		var = code[0];
		left_env = env;
		//判断是否是数组赋值
		if(code[1].value=="[")
		{
			WHUPout<<"find array "<<std::endl;
			isArr = true;
			while(thisarr<arrs.size())
			{
				if(arrs[thisarr].name==var.value)
				{
					WHUPout<<"find array "<<arrs[thisarr].name<<" on arrs index "<<thisarr<<std::endl;
					break;
				}
				thisarr++;
			}
			if(thisarr==arrs.size())
			{
				//兼顾了数组未声明的报错
				pushErrors(code[0], "Unrecognized arr token '" + var.value + "' ");
			}
		}

		var = code[0];//记录该变量
	
		if(!isArr)
		{
			// 两次处理将等号和等号左侧变量消去
			code.erase(code.begin());
			code.erase(code.begin());
		}
		else
		{
			code.erase(code.begin());//消去变量名
			int temp_dimension = arrs[thisarr].dimension;
			while(temp_dimension>0)
		    {
				for(auto i : code)
    			{
        			WHUPout<<i.value<<" ";
    			}
				int i=0;
				assign_matchBracket(code,i);
				WHUPout<<"i is "<<i<<std::endl;
			    // code.erase(code.begin());
			    // for(auto i:code)
    			// {
	    		// 	WHUPout<<i.value<<" ";
	    		// }
	    		Token temp1={SYMBOL,"*",code[0].line_number,code[0].file_name};
	    		Token temp2={SYMBOL,"+",code[0].line_number,code[0].file_name};
				Token temp3={SYMBOL,"(",code[0].line_number,code[0].file_name};
                Token temp4={SYMBOL,")",code[0].line_number,code[0].file_name};
				code.erase(code.begin());//消去左括号
				if(i>2)
				{
					indextokens.push_back(temp3);
					for(int j=0;j<i-1;j++)
					{
						indextokens.push_back(code[0]);
						code.erase(code.begin());
						// for(auto i : code)
    					// {
    					//     WHUPout<<i.value<<" ";
    					// }
						WHUPout<<std::endl;
					}
					indextokens.push_back(temp4);
				}
				else
				{
					indextokens.push_back(code[0]);
					code.erase(code.begin());
					// for(auto i : code)
    				// {
    				//     WHUPout<<i.value<<" ";
    				// }
					WHUPout<<std::endl;
				}
				code.erase(code.begin());//消去右括号
	    		indextokens.push_back(temp1);
                if(temp_dimension==1)
	    		{
                    indextokens.push_back({NUMBER,std::to_string(1),code[0].line_number,code[0].file_name});
	    		}else{
                    indextokens.push_back({NUMBER,std::to_string(arrs[thisarr].len[temp_dimension-2]),code[0].line_number,code[0].file_name});
                }
	    		indextokens.push_back(temp2);
	    		// WHUPout<<"index is "<<index<<std::endl;
	    		// code.erase(code.begin(),code.begin()+2);//删除前两个节点
	    		temp_dimension--;
	    	}
			indextokens.pop_back();//删除最后一个加号

			// index=std::stoi(code[2].value);
			// var.value+="[" + std::to_string(index) + "]";
			// //检查变量是否声明
			// checkSyntax::checkVar(var.value,env,code[0].line_number,code[0],true);
			////说明这是数组赋值,消去变量、中括号和等号
			// code.erase(code.begin(),code.begin()+4);

			code.erase(code.begin());//消去等号
		}
	}else{
	    left_env = namespace_table[code[0].value];
		var = code[2];
		code.erase(code.begin(),code.begin()+4);
	}
	WHUPout<<"right expr"<<std::endl;
	expr = new Expr(code,env);//将右侧代码建立新的expr
	WHUPout<<"expr tac.result"<<expr->getTacResult()<<std::endl;

	if(!isArr)
	{
	    //检查左边是否声明，未声明则自动声明
		if(env->get_var(var.value)=="null"){
			env->insert_var(var.value);
			env->change_type_var(var.value,expr->return_type());

			WHUPout << "var " << var.value << " declared automatically" << std::endl;
			WHUPout << "var " << var.value << " type is " << expr->return_type() << std::endl;
		
		}else{
			WHUPout<<"var "<<var.value<<" already declared"<<std::endl;

			WHUPout<<expr->getTacResult()<<" type is "<<expr->return_type();
			env->change_type_var(var.value,expr->return_type());//导致类型问题！！！

			WHUPout<<"var "<<var.value<<" type changed to "<<expr->return_type()<<std::endl;
		}
		assign();
	}
	else
	{
	    // //检查左边是否声明，未声明则自动声明
		// if(env->get_arr(var.value)=="null"){
		// 	env->insert_arr(var.value);
		// 	// env->change_type_var(var.value,expr->return_type());

		// 	WHUPout << "arr " << var.value << " declared automatically" << std::endl;
		//  WHUPout << "arr " << var.value << " type is " << expr->return_type() << std::endl;
		
		// }else{
			WHUPout<<"var "<<var.value<<" already declared"<<std::endl;

			WHUPout<<expr->getTacResult()<<" type is "<<"sub_arr "<<std::endl;
			// env->change_type_var(var.value,expr->return_type());//导致类型问题！！！

			// WHUPout<<"var "<<var.value<<" type changed to "<<expr->return_type()<<std::endl;
		// }

		Expr* index_expr = new Expr(indextokens,env);//将偏移量表达式建立新的expr
		WHUPout<<"index expr tac.result"<<index_expr->getTacResult()<<std::endl;
		assign(true,index_expr);
	}
}


void Assign::assign(bool isArr,Expr* index_expr)
{
	if(expr->return_type()=="string")
		tacs.push_back({STRASSIGN, "=",expr->getTacResult(),"",left_env->get_var(var.value)});
	else if(!isArr)
		tacs.push_back({ASSIGN,"=",expr->getTacResult(),"",left_env->get_var(var.value)});
	else	
		tacs.push_back({ASSIGN,"=",expr->getTacResult(),"",left_env->get_arr(var.value)+">->"+index_expr->getTacResult()});
}

void Assign::setEnv(Environment* env)
{
	this->env = env;
}

std::string Assign::return_type(){
	return expr->return_type();
}