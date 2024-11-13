#include <sstream>
#include <cmath>
#include"expression.h"
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
std::string infixToPostfix(const std::string& expr) {
    std::stack<char> ops;
    std::ostringstream postfix;
    for (char c : expr) {
        if (isspace(c)) continue;//跳过空格
        if (isdigit(c)) {
            postfix << c;//压入数据
        }
        else if (c == '(') {
            ops.push(c);
        }
        else if (c == ')') {
            while (!ops.empty() && ops.top() != '(') {
                postfix << ' ' << ops.top();
                ops.pop();
            }
            ops.pop();
        }
        else {
            while (!ops.empty() && precedence(ops.top()) >= precedence(c)) {
                postfix << ' ' << ops.top();
                ops.pop();
            }
            ops.push(c);
        }
    }
    while (!ops.empty()) {
        postfix << ' ' << ops.top();
        ops.pop();
    }
    return postfix.str();
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
    std::string postfix = infixToPostfix(expression);
    std::cout << postfix;
    //postfixToTAC(postfix);

    //// 打印三地址码
    //for (const auto& code : tac) {
    //    std::cout << code.result << " = " << code.arg1 << std::endl;
    //}

    return 0;
}