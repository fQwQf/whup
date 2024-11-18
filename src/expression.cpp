#include "expression.h"

extern std::vector<ThreeAddressCode> tacs; // 存储三地址代码的向量
extern int tempVarCounter;                 // 临时变量计数器

// 从右至左对输入进行遍历，扫描以下运算符，从下向上
/*
** | 幂运算
*, /, % | 乘，除，取余
+, - | 加和减
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
    if (expr.size() == 1)
    { // 只有一个元素
        if (expr[0].type == IDENTIFIER)
        {
            tac.result = env->get_var(E_expr[0].value);
        }
        else
        {
            tac.result = E_expr[0].value;
        }

        return;
    };
    this->setEnv(env);
    tac.result = newTempVar();
    this->expr();
} // 用表达式词法单元串初始化

std::string Expr::return_type()
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
void Expr::expr()
{

    // 扫描逻辑或
    for (int i = E_expr.size() - 1; i > 0; i--)
    {
        matchPar(i);
        if (E_expr[i].type == SYMBOL && E_expr[i].value == "||" || E_expr[i].type == KEYWORD && E_expr[i].value == "or")
        {
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

    // 前面均没扫到说明全部被括号包裹
    // 去掉首尾括号并重新调用expr（）
    E_expr.pop_back();
    E_expr.erase(E_expr.begin());
    this->expr();
};
