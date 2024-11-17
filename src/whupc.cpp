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

    std::string out;
    if(extractor.get_output_file() != ""){
        out = extractor.get_output_file();
    }else{
        out = "out.cpp";
    }

    IO io(extractor.get_input_file(), out);

    std::string expression = io.read();
    Lexer lexer(expression);

    std::vector<Token> tokens = lexer.tokenize();
    tokens.pop_back(); // 删除最后一个换行符

    Block block(tokens);

    std::string code = generator();

    io.write(code);

    std::cout << "Generate code to " << out << std::endl;
    std::cout << "Done!😉" << std::endl;

    return 0;
}
