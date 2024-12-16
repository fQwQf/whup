#include "expression.h"
#include "function.h"
#include"classfunction.h"
#include"object.h"
#include "check.h"
#include "arr.h"
#include "WHUPstream.h"

extern WHUPstream_compile1 WHUPout;

extern std::vector<ThreeAddressCode> tacs; // 存储三地址代码的向量

//不用哈希吗？
extern std::vector<Arr> arrs;       // 存储数组对象的向量

extern std::vector<ThreeAddressCode> tacArrs; // 存储数组三地址代码的向量
extern int tempVarCounter;                 // 临时变量计数器
extern std::unordered_map<std::string, Function*> functions;  // 存储函数名和对应的对象指针哈希表
extern std::unordered_map<std::string, Object*> object_table;  // 存储对象名和对应的对象指针哈希表
extern std::unordered_map<std::string,float>runtimeEnv_number;//
extern std::unordered_map<std::string,std::string>runtimeEnv_string;//
extern std::unordered_map<std::string, Environment*> namespace_table; // 存储命名空间名和对应的Environment对象的哈希表
// 从右至左对输入进行遍历，扫描以下运算符，从下向上
/*
    ** 表示 幂运算
    *  , /, % 表示 乘，除，取余
    +, - 表示 加和减
*/
// 返回扫描到的符号，并创建左右子树，以左边和右边的所有token传入所有构造
// 对于小括号的实现：可以增加一层扫描括号，并且在前两层调用一个略去括号的函数
// eg：对一个（1+2）直接进行操作，会先调用两次略去括号的函数，再开始扫描括号
void Expr::matchPar(int &i)
{
    if (E_expr[i].value == ")")
    {
        int leftPar = 0;
        int rightPar = 1; // 分别记录已经读取的左括号右括号的个数,当相等时即可结束
        while (leftPar != rightPar)
        {
            --i;
            if (E_expr[i].value == ")")
            {
                rightPar++;
            }
            else if (E_expr[i].value == "(")
            {
                leftPar++;
            }
            else
                continue;
        }
    }
}

void Expr::matchBracket(int &i)
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

bool Expr::is_single_arr(int demension)
{
    int leftPar = 0;
    int rightPar = 0;
    int this_Demension = 0;
    for(int i=0;i<E_expr.size();i++)
    {
        if(E_expr[i].value=="[")
        {
            leftPar++;
        }
        else if(E_expr[i].value=="]")
        {
            rightPar++;
        }
        else
            continue;
        if(leftPar==rightPar)
        {
            this_Demension++;
            leftPar=0;
            rightPar=0;
        }
    }
    if(this_Demension==demension)
        return true;
    else
        return false;
}

void Expr::setEnv(Environment *env)
{
    this->env = env;
}

Expr::Expr(const std::vector<Token> &expr, Environment *env) : E_expr(expr)
{
    
    this->setEnv(env);

    //这是命名空间中的变量
    if (expr.size() == 3 && expr[1].type == SYMBOL && expr[1].value == "::")
    {
        WHUPout << "find IDENTIFIER in namespace!";
        if (E_expr[2].processed == true)
        {
            tac.result = E_expr[2].value;
            WHUPout << "result: " << tac.result << std::endl;
            WHUPout << "processed: " << E_expr[2].processed << std::endl;
            return;
        }
        else
        {

            // 检查变量是否已经声明
            checkSyntax::checkVar(E_expr[2].value, namespace_table[expr[0].value], E_expr[2].line_number, E_expr[2]);

            tac.result = namespace_table[expr[0].value]->get_var(E_expr[2].value);
            WHUPout << "result: " << tac.result << std::endl;
            return;
        }
    }

    if (expr.size() == 1)
    { // 只有一个元素

        if (expr[0].type == IDENTIFIER)
        {
            WHUPout << "find IDENTIFIER!";
            if(E_expr[0].processed == true){
                tac.result = E_expr[0].value;
                WHUPout << "result: " << tac.result << std::endl;
                WHUPout << "processed: " << E_expr[0].processed << std::endl;
                return;
            }
          
            //检查变量是否已经声明
            checkSyntax::checkVar(E_expr[0].value,env,E_expr[0].line_number,E_expr[0]);
          
            tac.result = env->get_var(E_expr[0].value);
            WHUPout << "result: " << tac.result << std::endl;
        }
        else if (expr[0].type == STRING)
        {
            WHUPout << "find STRING!";
            tac.result = "\"" + E_expr[0].value + "\"";
            WHUPout << "result: " << tac.result << std::endl;
            runtimeEnv_string[tac.result]=tac.result;
            WHUPout<<runtimeEnv_string[tac.result]<<std::endl;
        }
        else if (expr[0].type == NUMBER)
        {
            WHUPout << "find NUMBER!";
            tac.result = E_expr[0].value;
            WHUPout << "result: " << tac.result << std::endl;

            runtimeEnv_number[tac.result]=std::stof(tac.result);
            //测试设置常量的思路
            //float啊！猜猜stoi的i代表什么？
        }
        return;
    };
    if(expr[0].type == IDENTIFIER && expr[1].value == "[" )
    {
        WHUPout << "find IDENTIFIER!";
        std::vector<Token> indextokens;
        int thisarr = 0;
        while(thisarr<arrs.size())
		{
			if(arrs[thisarr].name==E_expr[0].value)
			{
				WHUPout<<"find array "<<arrs[thisarr].name<<" on arrs index "<<thisarr<<std::endl;
				break;
			}
			thisarr++;
		}
		if(thisarr==arrs.size())
		{
			//兼顾了数组未声明的报错
			pushErrors(E_expr[0], "Unrecognized arr token '" + E_expr[0].value + "' ");
		}

        for(auto i : E_expr)
    	{
		    std::cout<<i.value<<" ";
    	}
		std::cout<<std::endl;
        int demension = arrs[thisarr].dimension;    
        int temp_dimension = arrs[thisarr].dimension;
        bool flag=is_single_arr(temp_dimension);
        std::cout << "is_single_arr flag: " << flag << std::endl;
        //说明只有这一个数组变量了，否则是数组表达式，不能直接运算，应该调用this->expr()
        if(flag)
        {
            E_expr.erase(E_expr.begin());//消去变量名
                // for(auto i : arrs)
            // {
            //     WHUPout<<"name = "<<i.name<<std::endl;
            //     WHUPout<<"dimension = "<<i.dimension<<std::endl;
            //     WHUPout<<"size = "<<i.size<<std::endl;
            // }
            // std::cout << "dimension: " << arrs[thisarr].dimension << std::endl;
            while(temp_dimension>0)
		    {
                int i=0;
                matchBracket(i);
                // Expr *expr = new Expr(std::vector<Token>(E_expr.begin()+1,E_expr.begin()+i-1), env);
            
			    // E_expr.erase(E_expr.begin());//消去左括号
			    // for(auto i:E_expr)
    			// {
	    		// 	WHUPout<<i.value<<" ";
	    		// }
                Token temp1={SYMBOL,"*",E_expr[0].line_number,E_expr[0].file_name};
	    		Token temp2={SYMBOL,"+",E_expr[0].line_number,E_expr[0].file_name};
                Token temp3={SYMBOL,"(",E_expr[0].line_number,E_expr[0].file_name};
                Token temp4={SYMBOL,")",E_expr[0].line_number,E_expr[0].file_name};
	    		// indextokens.push_back(E_expr[0]);
	    		E_expr.erase(E_expr.begin());//消去左括号
                if(i>2)
				{
					indextokens.push_back(temp3);
					for(int j=0;j<i-1;j++)
					{
						indextokens.push_back(E_expr[0]);
						E_expr.erase(E_expr.begin());
						// for(auto i : code)
    					// {
    					//     std::cout<<i.value<<" ";
    					// }
						std::cout<<std::endl;
					}
					indextokens.push_back(temp4);
				}
				else
				{
					indextokens.push_back(E_expr[0]);
					E_expr.erase(E_expr.begin());
					// for(auto i : code)
    				// {
    				//     std::cout<<i.value<<" ";
    				// }
					std::cout<<std::endl;
				}
				E_expr.erase(E_expr.begin());//消去右括号
	    		indextokens.push_back(temp1);
                if(temp_dimension==1)
	    		{
                    indextokens.push_back({NUMBER,std::to_string(1),E_expr[0].line_number,E_expr[0].file_name});
	    		}else{
                    indextokens.push_back({NUMBER,std::to_string(arrs[thisarr].len[temp_dimension-2]),E_expr[0].line_number,E_expr[0].file_name});
                }
	    		indextokens.push_back(temp2);
	    		// std::cout<<"index is "<<index<<std::endl;
	    		// E_expr.erase(E_expr.begin(),E_expr.begin()+2);//删除前两个节点
	    		temp_dimension--;
	    	}
            indextokens.pop_back();//删除最后一个加号
            Expr *index_expr = new Expr(indextokens, env);
            tac.result = env->get_arr(arrs[thisarr].name)+">->"+index_expr->getTacResult();
            // tac.result = env->get_arr(E_expr[0].value);
            WHUPout << "result: " << tac.result << std::endl;
            return;
        }
    }
    tac.result = newTempVar(return_type());
    //env->change_type_var(tac.result, return_type());
    this->expr();
} // 用表达式词法单元串初始化

//返回token类型的函数
std::string Expr::return_type()
{
    if (E_expr.size() == 1 && E_expr[0].type == IDENTIFIER)
    {
        return env->get_type_var(E_expr[0].value);
    }
    else
    {
        std::string type; // 返回类型
        for (int i = 0; i < E_expr.size(); i++)
        {
            if (E_expr[i].type == NUMBER)
            {
                if (type != "string")
                {
                    type = "number";
                }
            }
            if (E_expr[i].type == STRING)
            {
                type = "string";
            }
            else if (E_expr[i].type == SYMBOL && (E_expr[i].value == "<" || E_expr[i].value == "<=" || E_expr[i].value == ">" || E_expr[i].value == ">=" || E_expr[i].value == "==" || E_expr[i].value == "!="))
            {
                if (type != "string")
                {
                    type = "bool";
                }
            }
            else if (E_expr[i].type == IDENTIFIER && E_expr[i + 1].value != "(")
            {
                if (type != "string")
                {
                    type = env->get_type_var(E_expr[i].value);
                }
            }
            else if (E_expr[i].type == IDENTIFIER && E_expr[i + 1].value == "(")
            {
                Function *func = functions[E_expr[i].value];
                if (func != nullptr)
                {
                    if (type != "string")
                    {
                        type = func->return_type;
                    }
                }
                else
                {
                    // TODO:报错
                }
            }
        }
        return type;
    }
}

//合并对expr的所有处理,将得到的三地址码栈压入总栈
void Expr::expr()
{
    WHUPout << "expr scan start!" << "size:";
    WHUPout << E_expr.size() <<  "  ";
    for (auto &i : E_expr){
        WHUPout << i.value << " ";
    }
    WHUPout << std::endl;

    
    // 扫描逻辑或
    for (int i = E_expr.size() - 1; i > 0; i--)
    {
        matchPar(i);//扫描括号符号
        if (E_expr[i].type == SYMBOL && E_expr[i].value == "||" || E_expr[i].type == KEYWORD && E_expr[i].value == "or")
        {
            WHUPout << "find ||" << std::endl;

            left = new Expr(std::vector<Token>(E_expr.begin(), E_expr.begin() + i), this->env);
            left->env = env; // 传递环境
            tac.arg1 = left->tac.result;

            right = new Expr(std::vector<Token>(E_expr.begin() + i + 1, E_expr.end()), this->env);
            right->env = env;
            tac.arg2 = right->tac.result;
            tac.op = "||";
            tac.opperator=OR;
            tacs.push_back(tac);
            return;
        };
    };

    // 扫描逻辑与
    for (int i = E_expr.size() - 1; i > 0; i--)
    {
        matchPar(i);
        if (E_expr[i].type == SYMBOL && E_expr[i].value == "&&" || E_expr[i].type == KEYWORD && E_expr[i].value == "and")
        {
            WHUPout << "find &&" << std::endl;

            left = new Expr(std::vector<Token>(E_expr.begin(), E_expr.begin() + i), this->env);
            left->env = env; // 传递环境
            tac.arg1 = left->tac.result;

            right = new Expr(std::vector<Token>(E_expr.begin() + i + 1, E_expr.end()), this->env);
            right->env = env;
            tac.arg2 = right->tac.result;
            tac.opperator=AND;
            tac.op = "&&";
            tacs.push_back(tac);
            return;
        };
    };

    // 扫描比较符
    // 幸运的是，c++中int完全兼容bool，因此没有必要区分
    for (int i = E_expr.size() - 1; i > 0; i--)
    {
        matchPar(i);
        if (E_expr[i].type == SYMBOL && (E_expr[i].value == "<" || E_expr[i].value == "<=" || E_expr[i].value == ">" || E_expr[i].value == ">=" || E_expr[i].value == "==" || E_expr[i].value == "!="))
        {
            WHUPout << "find compare" << std::endl;

            left = new Expr(std::vector<Token>(E_expr.begin(), E_expr.begin() + i), this->env);
            left->env = env; // 传递环境
            tac.arg1 = left->tac.result;

            right = new Expr(std::vector<Token>(E_expr.begin() + i + 1, E_expr.end()), this->env);
            right->env = env;
            tac.arg2 = right->tac.result;
            if (E_expr[i].value == "<")
            {
                tac.opperator=LT;
            }
            else if (E_expr[i].value == "<=")
            {
                tac.opperator=LE;
            }
            else if (E_expr[i].value == ">")
            {
                tac.opperator=GT;
            }
            else if (E_expr[i].value == ">=")
            {
                tac.opperator=GE;
            }
            else if (E_expr[i].value == "==")
            {
                tac.opperator=EQ;
            }
            else if (E_expr[i].value == "!=")
            {
                tac.opperator=NEQ;
            }
            tac.op = E_expr[i].value;
            tacs.push_back(tac);
            return;
        };
    };

    // 扫描加和减
    for (int i = E_expr.size() - 1; i > 0; i--)
    {
        matchPar(i);
        if(E_expr[i].type == SYMBOL && E_expr[i].value == "]")
        {
            int leftPar = 0;
            int rightPar = 1; // 分别记录已经读取的左括号右括号的个数,当相等时即可结束
            while (leftPar != rightPar)
            {
                --i;
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
        if (E_expr[i].type == SYMBOL && (E_expr[i].value == "+" || E_expr[i].value == "-"))
        {
            //c++不支持两个直接的string相加，故这样处理
            if (E_expr.size() == 3 && E_expr[0].type == STRING && E_expr[2].type == STRING && E_expr[1].value == "+")
            {
                WHUPout << "find string add" << std::endl;

                std::string temp = newTempVar("string");
                tacs.push_back({STRASSIGN,"=", "\"" + E_expr[0].value + "\"", "", temp});
                right = new Expr(std::vector<Token>(E_expr.begin() + i + 1, E_expr.end()), this->env);
                right->env = env;

                tac.arg1 = temp;
                tac.arg2 = right->tac.result;
                tac.opperator=STRADD;
                tac.op = E_expr[i].value;
                tacs.push_back(tac);
                return;
            }

            WHUPout << "find add   i =" << i <<std::endl;

            left = new Expr(std::vector<Token>(E_expr.begin(), E_expr.begin() + i), this->env);
            left->env = env; // 传递环境
            tac.arg1 = left->tac.result;

            right = new Expr(std::vector<Token>(E_expr.begin() + i + 1, E_expr.end()), this->env);
            right->env = env;
            tac.arg2 = right->tac.result;
            if(E_expr[i].value == "+")
            {
                tac.opperator=ADD;
            }
            else if(E_expr[i].value == "-")
            {
                tac.opperator=SUB;
            }
            tac.op = E_expr[i].value;
            tacs.push_back(tac);
            return;
        };
    };

    // 扫描乘，除，取余
    for (int i = E_expr.size() - 1; i > 0; i--)
    {
        matchPar(i);
        if (E_expr[i].type == SYMBOL && (E_expr[i].value == "*" || E_expr[i].value == "/" || E_expr[i].value == "%"))
        {
            WHUPout << "find */%" << std::endl;

            left = new Expr(std::vector<Token>(E_expr.begin(), E_expr.begin() + i), this->env);
            left->env = env; // 传递环境
            tac.arg1 = left->tac.result;

            right = new Expr(std::vector<Token>(E_expr.begin() + i + 1, E_expr.end()), this->env);
            right->env = env;
            tac.arg2 = right->tac.result;
            if(E_expr[i].value == "*")
            {
                tac.opperator=MUL;
            }
            else if(E_expr[i].value == "/")
            {
                tac.opperator=DIV;
            }
            else if(E_expr[i].value == "%")
            {
                tac.opperator=MOD;
            }
            tac.op = E_expr[i].value;
            tacs.push_back(tac);
            return;
        };
    };

    //这就是函数调用
    //扫描乘方
    //乘方结合性与其他的不同，扫描方向也相反
    for (int i = 0; i < E_expr.size() - 1; i++){

        // 反方向的matchPar
        if (E_expr[i].value == "(")
        {
            int leftPar = 1;
            int rightPar = 0;
            while (leftPar != rightPar)
            {
                ++i;
                if (E_expr[i].value == ")")
                {
                    rightPar++;
                }
                else if (E_expr[i].value == "(")
                {
                    leftPar++;
                }
                else
                    continue;
            }
        }

        if (E_expr[i].type == SYMBOL && E_expr[i].value == "**")
        {
            WHUPout << "find **" << std::endl;

            left = new Expr(std::vector<Token>(E_expr.begin(), E_expr.begin() + i), this->env);
            left->env = env; // 传递环境
            tac.arg1 = left->tac.result;

            right = new Expr(std::vector<Token>(E_expr.begin() + i + 1, E_expr.end()), this->env);
            right->env = env;
            tac.arg2 = right->tac.result;
            tac.opperator=POW;
            tac.op = E_expr[i].value;
            tacs.push_back(tac);
            return;
        };
    }

    //这就是函数调用，能扫到这里说明前面都没扫到
    Environment* funcenv = env;
    if (E_expr[0].type == IDENTIFIER && E_expr[1].type == SYMBOL && E_expr[1].value == "::") {
        // 对于命名空间，先转化为没有命名空间的情况
        funcenv = namespace_table[E_expr[0].value];
        E_expr.erase(E_expr.begin(), E_expr.begin()+2);
    }
    
    if (E_expr[0].type == IDENTIFIER && E_expr[1].type == SYMBOL && E_expr[1].value == "(" && E_expr[E_expr.size() - 1].type == SYMBOL && E_expr[E_expr.size() - 1].value == ")"){
        Function* func = funcenv->get_function(E_expr[0].value);
        std::vector<Token> E_expression = E_expr;
        std::string temp = newTempVar(func->return_type);
        this->env->insert_return_var(temp);
        func->call(E_expression,env);

        if(func->return_type=="string")
        tacs.push_back({STRASSIGN, "=", func->get_return_value(), "", temp });
        else
        tacs.push_back({ASSIGN, "=", func->get_return_value(), "", temp });
        tac.result = temp;
        return;
    }
    if(E_expr[0].type==IDENTIFIER&&E_expr[1].value=="->"){
        std::string objectName = E_expr[0].value;
        std::string functionName = E_expr[2].value;
        std::vector<Token> expression = E_expr;
        Object* object = object_table[objectName];
        std::unordered_map<std::string,ClassFunction*> function_table = object->function_table;
        ClassFunction* object_function = function_table[functionName];
        std::string temp = newTempVar(object_function->return_type);
        this->env->insert_return_var(temp);
        object_function->call(expression,env);

        if(object_function->return_type=="string")
        tacs.push_back({STRASSIGN, "=", object_function->get_return_value(), "", temp });
        else
        tacs.push_back({ASSIGN, "=", object_function->get_return_value(), "", temp });
        tac.result =temp;
        return;
    }
    //TODO 函数调用找不到说明未定义，报错
    

    // 前面均没扫到说明全部被括号包裹
    // 去掉首尾括号并重新调用expr（）
    WHUPout << "find par" << std::endl;

    //检查是否出现错误
    // printErrors();

    E_expr.pop_back();
    E_expr.erase(E_expr.begin());
    this->expr();
    WHUPout<<"expr success."<<std::endl;
};
