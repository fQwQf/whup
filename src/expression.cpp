#include "expression.h"
#include "function.h"
#include"classfunction.h"
#include"object.h"
#include "check.h"

extern std::vector<ThreeAddressCode> tacs; // 存储三地址代码的向量
extern int tempVarCounter;                 // 临时变量计数器
extern std::unordered_map<std::string, Function*> functions;  // 存储函数名和对应的对象指针哈希表
extern std::unordered_map<std::string, Object*> object_table;  // 存储对象名和对应的对象指针哈希表
extern std::unordered_map<std::string,float>runtimeEnv_number;//
extern std::unordered_map<std::string,std::string>runtimeEnv_string;//
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

void Expr::setEnv(Environment *env)
{
    this->env = env;
}

Expr::Expr(const std::vector<Token> &expr, Environment *env) : E_expr(expr)
{
    
    this->setEnv(env);
    if (expr.size() == 1)
    { // 只有一个元素

        if (expr[0].type == IDENTIFIER)
        {
            std::cout << "find IDENTIFIER!";
            if(E_expr[0].processed == true){
                tac.result = E_expr[0].value;
                std::cout << "result: " << tac.result << std::endl;
                std::cout << "processed: " << E_expr[0].processed << std::endl;
                return;
            }
          
            //检查变量是否已经声明
            checkSyntax::checkVar(E_expr[0].value,env,E_expr[0].line_number,E_expr[0]);
          
            tac.result = env->get_var(E_expr[0].value);
            std::cout << "result: " << tac.result << std::endl;
        }
        else if (expr[0].type == STRING)
        {
            std::cout << "find STRING!";
            tac.result = "\"" + E_expr[0].value + "\"";
            std::cout << "result: " << tac.result << std::endl;
        }
        else if (expr[0].type == NUMBER)
        {
            std::cout << "find NUMBER!";
            tac.result = E_expr[0].value;
            std::cout << "result: " << tac.result << std::endl;

            runtimeEnv_number[tac.result]=std::stoi(tac.result);//测试设置常量的思路
        }
        return;
    };
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
        for (auto i : E_expr)
        {
            if (i.type == STRING)
            {
                return "string";
            }
            else if (i.type == SYMBOL && (i.value == "<" || i.value == "<=" || i.value == ">" || i.value == ">=" || i.value == "==" || i.value == "!="))
            {
                return "bool";
            }
        }
        return "number";
    }
}

//合并对expr的所有处理,将得到的三地址码栈压入总栈
void Expr::expr()
{
    std::cout << "expr scan start!" << "size:";
    std::cout << E_expr.size() <<  "  ";
    for (auto &i : E_expr){
        std::cout << i.value << " ";
    }
    std::cout << std::endl;

    
    // 扫描逻辑或
    for (int i = E_expr.size() - 1; i > 0; i--)
    {
        matchPar(i);//扫描括号符号
        if (E_expr[i].type == SYMBOL && E_expr[i].value == "||" || E_expr[i].type == KEYWORD && E_expr[i].value == "or")
        {
            std::cout << "find ||" << std::endl;

            left = new Expr(std::vector<Token>(E_expr.begin(), E_expr.begin() + i), this->env);
            left->env = env; // 传递环境
            tac.arg1 = left->tac.result;

            right = new Expr(std::vector<Token>(E_expr.begin() + i + 1, E_expr.end()), this->env);
            right->env = env;
            tac.arg2 = right->tac.result;

            tac.op = "||";
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
            std::cout << "find &&" << std::endl;

            left = new Expr(std::vector<Token>(E_expr.begin(), E_expr.begin() + i), this->env);
            left->env = env; // 传递环境
            tac.arg1 = left->tac.result;

            right = new Expr(std::vector<Token>(E_expr.begin() + i + 1, E_expr.end()), this->env);
            right->env = env;
            tac.arg2 = right->tac.result;

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
            std::cout << "find compare" << std::endl;

            left = new Expr(std::vector<Token>(E_expr.begin(), E_expr.begin() + i), this->env);
            left->env = env; // 传递环境
            tac.arg1 = left->tac.result;

            right = new Expr(std::vector<Token>(E_expr.begin() + i + 1, E_expr.end()), this->env);
            right->env = env;
            tac.arg2 = right->tac.result;

            tac.op = E_expr[i].value;
            tacs.push_back(tac);
            return;
        };
    };

    // 扫描加和减
    for (int i = E_expr.size() - 1; i > 0; i--)
    {
        matchPar(i);
        if (E_expr[i].type == SYMBOL && (E_expr[i].value == "+" || E_expr[i].value == "-"))
        {
            //c++不支持两个直接的string相加，故这样处理
            if (E_expr.size() == 3 && E_expr[0].type == STRING && E_expr[2].type == STRING && E_expr[1].value == "+")
            {
                std::cout << "find string add" << std::endl;

                std::string temp = newTempVar("string");
                tacs.push_back({"=", "\"" + E_expr[0].value + "\"", "", temp});
                right = new Expr(std::vector<Token>(E_expr.begin() + i + 1, E_expr.end()), this->env);
                right->env = env;

                tac.arg1 = temp;
                tac.arg2 = right->tac.result;

                tac.op = E_expr[i].value;
                tacs.push_back(tac);
                return;
            }

            std::cout << "find add" << std::endl;

            left = new Expr(std::vector<Token>(E_expr.begin(), E_expr.begin() + i), this->env);
            left->env = env; // 传递环境
            tac.arg1 = left->tac.result;

            right = new Expr(std::vector<Token>(E_expr.begin() + i + 1, E_expr.end()), this->env);
            right->env = env;
            tac.arg2 = right->tac.result;

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
            std::cout << "find */%" << std::endl;

            left = new Expr(std::vector<Token>(E_expr.begin(), E_expr.begin() + i), this->env);
            left->env = env; // 传递环境
            tac.arg1 = left->tac.result;

            right = new Expr(std::vector<Token>(E_expr.begin() + i + 1, E_expr.end()), this->env);
            right->env = env;
            tac.arg2 = right->tac.result;

            tac.op = E_expr[i].value;
            tacs.push_back(tac);
            return;
        };
    };

    //这就是函数调用
    if (E_expr[0].type == IDENTIFIER && E_expr[1].type == SYMBOL && E_expr[1].value == "(" && E_expr[E_expr.size() - 1].type == SYMBOL && E_expr[E_expr.size() - 1].value == ")"){
        Function* func = functions[E_expr[0].value];
        std::vector<Token> E_expression = E_expr;
        std::string temp = newTempVar(func->return_type);
        this->env->insert_return_var(temp);
        func->call(E_expression,env);
        tacs.push_back({ "=", func->get_return_value(), "", temp });
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
        object_function->call(expression,env);
        tac.result =object_function->get_return_value();
        std::cout << "call function: " << E_expr[0].value << std::endl;
        return;
    }

    // 前面均没扫到说明全部被括号包裹
    // 去掉首尾括号并重新调用expr（）
    std::cout << "find par" << std::endl;

    //检查是否出现错误
    printErrors();

    E_expr.pop_back();
    E_expr.erase(E_expr.begin());
    this->expr();
    std::cout<<"expr success.";
};
