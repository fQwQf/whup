#include <iostream>
#include <stack>
#include <string>

int precedence(char op) {
    switch (op) {
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
        case '^': return 3;
        default: return -1;
    }
}

std::string infixToPostfix(const std::string& infix) {
    std::stack<char> operators;
    std::string postfix;

    for (char ch : infix) {
        if (isalnum(ch)) { // 如果是字母或数字，直接添加到后缀表达式
            postfix += ch;
        } else if (ch == '(') { // 如果是左括号，压入栈
            operators.push(ch);
        } else if (ch == ')') { // 如果是右括号，弹出栈顶直到遇到左括号
            while (!operators.empty() && operators.top() != '(') {
                postfix += operators.top();
                operators.pop();
            }
            if (!operators.empty() && operators.top() == '(') {
                operators.pop(); // 弹出左括号
            }
        } else { // 如果是运算符
            while (!operators.empty() && precedence(operators.top()) >= precedence(ch)) {
                postfix += operators.top();
                operators.pop();
            }
            operators.push(ch);
        }
    }

    // 弹出栈中剩余的所有运算符
    while (!operators.empty()) {
        postfix += operators.top();
        operators.pop();
    }

    return postfix;
}



int main() {
    std::string infix = "3+4*2/(1-5)";
    std::string postfix = infixToPostfix(infix);
    std::cout << "中缀表达式: " << infix << std::endl;
    std::cout << "后缀表达式: " << postfix << std::endl;
    return 0;
}