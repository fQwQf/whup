#include "block.h"
#include "expression.h"


extern std::vector<ThreeAddressCode> tacs;  // 存储三地址代码的向量
extern int tempVarCounter;  // 临时变量计数器

int main() {
    std::string expression = "a = (3+2)-(8+7)*2;b = 3;c = a + b;d = 4+5*a/c;";
    Lexer lexer(expression);
    std::vector<Token> tokens = lexer.tokenize();
    tokens.pop_back(); // 删除最后一个换行符


    Block block(tokens);

    // 打印三地址码
    for (const auto& code : tacs) {
        std::cout << code.result << " = " << code.arg1
            << code.op << code.arg2 << std::endl;
    }

    return 0;  
}