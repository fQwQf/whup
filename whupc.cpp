#include "expression.h"

extern std::vector<ThreeAddressCode> tacs;  // 存储三地址代码的向量
extern int tempVarCounter;  // 临时变量计数器

int main() {
    std::string expression = "(3+2)-(8+7)*2";
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

    // 打印三地址码
    for (const auto& code : tacs) {
        std::cout << code.result << " = " << code.arg1
            << code.op << code.arg2 << std::endl;
        std::cout << "-----------------" << std::endl;
    }

    return 0;  
}