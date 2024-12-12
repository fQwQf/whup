#include "block.h"
#include "expression.h"
#include "whup_io.h"
#include "extractor.h"
#include "generator.h"
#include "check.h"
#include"execute.h"
#include<unordered_map>
#include "lexer.h"

#ifdef _WIN32
#include "windows.h"
#endif


extern std::vector<ThreeAddressCode> tacArrs;  
extern std::vector<ThreeAddressCode> tacs;  // 存储三地址代码的向量
extern std::vector<std::pair<std::string, std::string>> var_declares;//存储变量的声明信息
extern int tempVarCounter;  // 临时变量计数器
extern std::unordered_map<std::string,float>runtimeEnv_number;//
int main(int n, const char *arg[])
{
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif

    if(n==1){
        std::cout << "Usage: whupc <input_file> | -i <input_file> | -o <output_file>" << std::endl;
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

    //用io类读取输入文件内容到字符串expression中
    IO io(extractor.get_input_file(), out);
    std::string expression = io.read();

    //进行expression的句法错误分析
    //CheckSemicolon::checkCode(expression,extractor.get_input_file());
    //printErrors();

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

    //生成目标代码
    //std::string code = generator();

    //将目标代码写入输出文件
    //io.writeTAC(tacs);

    io.writeTAC(tacs);

    std::cout << "Generate code to " << out << std::endl;
    std::cout << "\033[0;32m Done!ヾ(•ω•`)o \033[0m" << std::endl;

    // std::cout<<"tacArrs size:" << tacArrs.size() << std::endl;
    // for(auto i : tacArrs)
    // {
    //     std::cout << i.arg1 << std::endl;
    //     std::cout << i.arg2 << std::endl;
    //     std::cout << i.opperator << std::endl;
    //     std::cout << i.result << std::endl;
    // }
    std::vector<runTAC> runtacs = TAC_to_runTAC(tacs);//将tacs转换为runTAC

    for (auto i : runtacs){
        std::cout << "arg1:" << i.arg1 << " arg2:" << i.arg2 << " op:" << i.opperator << " result:" << i.result <<" line:" << i.line << std::endl;
        if (i.arg1!=0){
            std::cout << "arg1: " << *(float*)i.arg1 << std::endl;
        }
        if (i.arg2!=0){
            std::cout << "arg2: " << *(float*)i.arg2 << std::endl;
        }
        if (i.result!=0){
            std::cout << "result: " << *(float*)i.result << std::endl;
        }
        std::cout << std::endl;
    }

    std::clock_t start = clock();
    
    execute(runtacs);

    std::clock_t end   = clock();

    if (extractor.wall_clock)
        std::cout << "Wall clock time:" << (double)(end - start) / CLOCKS_PER_SEC << "s" << std::endl;

    std::cout<<"Execute success!"<<std::endl;
    return 0;
}
