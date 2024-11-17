#include "block.h"
#include "expression.h"
#include "whup_io.h"
#include "extractor.h"
#include "generator.h"


extern std::vector<ThreeAddressCode> tacs;  // 存储三地址代码的向量
extern std::vector<std::pair<std::string, std::string>> var_declares;
extern int tempVarCounter;  // 临时变量计数器

int main(int n, const char *arg[])
{
    Extractor extractor(n, arg);
    IO io(extractor.get_input_file(), extractor.get_output_file());

    std::string expression = io.read();
    Lexer lexer(expression);
    std::vector<Token> tokens = lexer.tokenize();
    tokens.pop_back(); // 删除最后一个换行符

    Block block(tokens);

    // 打印三地址码
    //std::string code = generator();

    //std::cout << code << std::endl;
    return 0;
}