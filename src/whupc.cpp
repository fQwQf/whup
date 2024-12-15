#include "block.h"
#include "expression.h"
#include "whup_io.h"
#include "extractor.h"
#include "generator.h"
#include "check.h"
#include"execute.h"
#include <unordered_map>
#include "lexer.h"
#include "WHUPstream.h"

#ifdef _WIN32
#include "windows.h"
#endif

extern std::vector<ThreeAddressCode> tacArrs;  
extern std::vector<ThreeAddressCode> tacs;  // 存储三地址代码的向量
extern std::unordered_map<std::string,float>runtimeEnv_number;//

extern bool print_c1;
extern bool print_c2;
extern bool print_e;
int main(int n, const char *arg[])
{
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif

    if(n==1){
        std::cout << "Usage:" << " whupc " << std::endl
                << std::left << std::setw(25) << " <file>" << " input file path" << std::endl
                << std::left << std::setw(25) << " -i / --input <file>" << " input file path" << std::endl
                << std::left << std::setw(25) << " -o / --output <file>" << " output file path (default: out.hust)"<< std::endl
                << std::left << std::setw(25) << " -pc1 / --print-c1" << " print details of the first compile" << std::endl;
        return 0;
    }


    Extractor extractor(n, arg);//提取输入和输出文件路径

    //确定文件的输出路径，默认为out.cpp
    std::string out;
    if(extractor.get_output_file() != ""){
        out = extractor.get_output_file();
    }else{
        out = "out.hust";
    }

    //确定是否打印信息
    print_c1 = extractor.print_c1;

    //用io类读取输入文件内容到字符串expression中
    IO io(extractor.get_input_file(), out);
    std::string expression = io.read();

    //进行expression的句法错误分析
    CheckSemicolon::checkCode(expression,extractor.get_input_file());
    printErrors();

    //先对tokens进行统一的括号错误检查
    checkBrackets::checkPar(expression,extractor.get_input_file());
    checkBrackets::checkBracket(expression,extractor.get_input_file());
    checkBrackets::checkBrace(expression,extractor.get_input_file());
    printErrors();

    //进行词法分析
    Lexer lexer(expression,extractor.get_input_file());
    std::vector<Token> tokens = lexer.tokenize();
    tokens.pop_back(); // 删除最后一个换行符


    //使用得到的token集合进行语法分析，生成一个中间表示
    Block block(tokens);

    //进行expression的语法错误检查，并输出错误信息
    printErrors();

    //将目标代码写入输出文件
    io.writeTAC();

    std::cout << "Generate code to " << out << std::endl;
    std::cout  << "\033[0;32m Done!ヾ(•ω•`)o \033[0m" << std::endl;

    return 0;
}
