//#include <sstream>
//#include <cmath>
#include <bits/stdc++.h>
#include "expression.h"

//#include"lexer.cpp"

/*
*结构：三地址码
*
*储存四个字符串，分别代表运算符，操作数1，操作数2，结果
*/
struct ThreeAddressCode {
    std::string op;   // 操作符
    std::string arg1;
    std::string arg2;
    std::string result;
};

std::vector<ThreeAddressCode> tac;  // 存储三地址代码的向量
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

// 将中缀表达式转换为后缀表达式
std::queue<Token> infixToPostfix(const std::vector<Token>& expr) {
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
}

// 将后缀表达式转换为三地址码
void postfixToTAC(const std::string& postfix) {
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
}

int main() {
    std::string expression = "3 + 4 * 2 / ( 1 - 5 ) ";
    Lexer lexer(expression);
    std::vector<Token> tokens = lexer.tokenize();
    for (const auto& token : tokens) {
       std::cout << "Type: " << static_cast<int>(token.type) << ", Value: " << token.value 
                 << ", Line: " << token.line_number  << std::endl;
   }
    std::queue<Token> postfix = infixToPostfix(tokens);
    for (int i=0; i<postfix.size(); i++){
        
        std::cout << postfix.front().value << " ";
        postfix.pop();
    }
    //postfixToTAC(postfix);

    //// 打印三地址码
    /*for (const auto& code : tac) {
        std::cout << code.result << " = " << code.arg1 << std::endl;
    }*/

    return 0;
}