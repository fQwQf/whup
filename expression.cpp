//#include <sstream>
//#include <cmath>
#include "expression.h"

//#include"lexer.cpp"



std::vector<ThreeAddressCode> tacs;  // 存储三地址代码的向量
int tempVarCounter = 0;  // 临时变量计数器

std::string newTempVar() {
    return "t" + std::to_string(++tempVarCounter);
}

// 比较运算符优先级
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}




//从右至左对输入进行遍历，扫描以下运算符，从下向上
/*
** | 幂运算
*, /, % | 乘，除，取余
+, - | 加和减
*/
//返回扫描到的符号，并创建左右子树，以左边和右边的所有token传入所有构造
//对于小括号的实现：可以增加第三层扫描括号，并且在前两层调用一个略去括号的函数
// 待实现：多层括号的处理
//eg：对一个（1+2）直接进行操作，会先调用两次略去括号的函数，再开始扫描括号
void Expr::matchPar(int& i)
{
    if (E_expr[i].value == ")")
    {
        int leftPar = 0;
        int rightPar = 1;//分别记录已经读取的左括号右括号的个数,当相等时即可结束
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


void Expr::expr() {
    //第一层：扫描加和减
    for (int i = E_expr.size() - 1; i > 0; i--) {
        matchPar(i);
        if (E_expr[i].type == SYMBOL && (E_expr[i].value == "+" || E_expr[i].value == "-")) {
            if (i == 1) {
                tac.arg1 = E_expr[i - 1].value;
            }
            else {
                left = new Expr(std::vector<Token>(E_expr.begin(), E_expr.begin() + i));
                left->expr();
                tac.arg1 = left->tac.result;
            };

            if (i == E_expr.size() - 2) {
                tac.arg2 = E_expr[i + 1].value;
            }
            else {
                right = new Expr(std::vector<Token>(E_expr.begin() + i + 1, E_expr.end()));
                right->expr();
                tac.arg2 = right->tac.result;
            };

            tac.op = E_expr[i].value;
            tacs.push_back(tac);
            return;
        };
    };

    //第二层：扫描乘，除，取余
    for (int i = E_expr.size() - 1; i > 0; i--) {
        matchPar(i);
        if (E_expr[i].type == SYMBOL && (E_expr[i].value == "*" || E_expr[i].value == "/" || E_expr[i].value == "%")) {
            if (i == 1) {
                tac.arg1 = E_expr[i - 1].value;
            }
            else {
                left = new Expr(std::vector<Token>(E_expr.begin(), E_expr.begin() + i));
                left->expr();
                tac.arg1 = left->tac.result;
            };

            if (i == E_expr.size() - 2) {
                tac.arg2 = E_expr[i + 1].value;
            }
            else {
                right = new Expr(std::vector<Token>(E_expr.begin() + i + 1, E_expr.end()));
                right->expr();
                tac.arg2 = right->tac.result;
            };
            tac.op = E_expr[i].value;
            tacs.push_back(tac);
            return;
        };
    };
    //第三层：去掉括号并调用expr（）从第一层开始重新解析
    Expr* child = new Expr(std::vector<Token>(E_expr.begin() + 1, E_expr.end() - 1));
    child->expr();
    tac.op = " ";
    tac.arg1 = child->tac.result;
    tac.arg2 = " ";
    tacs.push_back(tac);
};

int main() {
    std::string expression = "(1+1)+2*3";
    Lexer lexer(expression);
    std::vector<Token> tokens = lexer.tokenize();
    tokens.pop_back(); // 删除最后一个换行符
    for (const auto& token : tokens) {
        std::cout << "Type: " << static_cast<int>(token.type) << ", Value: " << token.value
            << ", Line: " << token.line_number << std::endl;
    }

    Expr expr(tokens);
    expr.expr();
    //std::cout << expr.tac.result << std::endl;
    //std::cout << expr.tac.arg1 << std::endl;
    //std::cout << expr.tac.arg2 << std::endl;
    //postfixToTAC(postfix);

    //// 打印三地址码
    for (const auto& code : tacs) {
        std::cout << code.result << " = " << code.arg1
            << code.op << code.arg2 << std::endl;
        std::cout << "-----------------" << std::endl;
    }

    return 0;
}