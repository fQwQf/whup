//#include <sstream>
//#include <cmath>
#include <bits/stdc++.h>
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
void Expr::expr() {
    //第一层：扫描加和减
    for (int i = E_expr.size()- 1; i >0; i--) {
        if (E_expr[i].type == SYMBOL && (E_expr[i].value == "+" || E_expr[i].value == "-")) {
            if(i == 1){
                tac.arg1 = E_expr[i-1].value;
            }else{
                left = new Expr(std::vector<Token>(E_expr.begin(), E_expr.begin() + i));
                left->expr();
                tac.arg1 = left->tac.result;
            };

            if(i == E_expr.size() - 2){
                tac.arg2 = E_expr[i+1].value;
            }else{
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
    for (int i = E_expr.size()- 1; i >0; i--) {
        if (E_expr[i].type == SYMBOL && (E_expr[i].value == "*" || E_expr[i].value == "/" || E_expr[i].value == "%")) {
            if(i == 1){
                tac.arg1 = E_expr[i-1].value;
            }else{
                left = new Expr(std::vector<Token>(E_expr.begin(), E_expr.begin() + i));
                left->expr();
                tac.arg1 = left->tac.result;
            };

            if(i == E_expr.size() - 2){
                tac.arg2 = E_expr[i+1].value;
            }else{
                right = new Expr(std::vector<Token>(E_expr.begin() + i + 1, E_expr.end()));
                right->expr();
                tac.arg2 = right->tac.result;
            };

        tac.op = E_expr[i].value;
        tacs.push_back(tac);
        return;
        };
    };

};








// 将中缀表达式转换为后缀表达式
/*std::queue<Token> infixToPostfix(const std::vector<Token>& expr) {
    std::stack<Token> ops;
    std::queue<Token> postfix;
    for (Token c : expr) {
        if (c.type == IDENTIFIER || c.type == NUMBER) {
            postfix.push(c);//压入数据
        }
        else if (c.value == "(") {
            ops.push(c);
        }
        else if (c.value == ")") {
            while (!ops.empty() && ops.top().value != "(") {
                postfix.push(ops.top());
                ops.pop();
            }
            ops.pop();
        }
        else {
            while (!ops.empty() && precedence(ops.top().value[0]) >= precedence(c.value[0])) {
                postfix.push(ops.top());
                ops.pop();
            }
            ops.push(c);
        }
    }
    while (!ops.empty()) {
        postfix.push(ops.top());
        ops.pop();
    }

    return postfix;
}*/

// 将后缀表达式转换为三地址码
/*void postfixToTAC(const std::string& postfix) {
    std::istringstream iss(postfix);
    std::stack<std::string> values;
    std::string token;
    while (iss >> token) {
        if (isdigit(token[0])) {
            tac.push_back({ "=", token, "", newTempVar() });
            values.push(newTempVar());
        }
        else {
            std::string op1 = values.top();
            values.pop();
            std::string op2 = values.top();
            values.pop();
            std::string result = newTempVar();
            tac.push_back({ "=", op2 + " " + token + " " + op1, "", result });
            values.push(result);
        }
    }
}*/

int main() {
    std::string expression = "3 + 4 * 2 /  1 - 5 ";
    Lexer lexer(expression);
    std::vector<Token> tokens = lexer.tokenize();
    tokens.pop_back(); // 删除最后一个换行符
    for (const auto& token : tokens) {
       std::cout << "Type: " << static_cast<int>(token.type) << ", Value: " << token.value 
                 << ", Line: " << token.line_number  << std::endl;
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