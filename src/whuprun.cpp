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

extern std::vector<ThreeAddressCode> tacs;  // 存储三地址代码的向量
extern std::vector<ThreeAddressCode> tacArrs;  
extern std::unordered_map<std::string, std::string> var_declares;//存储变量的声明信息
extern std::unordered_map<std::string,float>runtimeEnv_number;//
extern std::unordered_map<std::string,std::string>runtimeEnv_string;//

extern bool print_c1;
extern bool print_c2;
extern bool print_e;

extern WHUPstream_compile1 WHUPout;
extern WHUPstream_compile2 WHUPout_c2;
extern WHUPstream_execute WHUPout_e;


int main(int n, const char *arg[])
{
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif

    if(n==1){
        std::cout << "Usage: whuprun " << std::endl
                << std::left << std::setw(25) << " <file>" << " input file path" << std::endl
                << std::left << std::setw(25) << " -i / --input <file>" << " input file path" << std::endl
                << std::left << std::setw(25) << " -c / --clock" << " print wall clock time" << std::endl
                << std::left << std::setw(25) << " -pc1 / --print-c1" << " print details of the first compile" << std::endl
                << std::left << std::setw(25) << " -pc2 / --print-c2" << " print details of the second compile" << std::endl
                << std::left << std::setw(25) << " -pe / --print-e" << " print details of execute" << std::endl;
        return 0;
    }

    Extractor extractor(n, arg);//提取输入文件路径

    //确定是否打印信息
    print_c1 = extractor.print_c1;
    print_c2 = extractor.print_c2;
    print_e = extractor.print_e;

    //用io类读取输入文件内容到字符串expression中
    IO io(extractor.get_input_file());
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

    WHUPout << "\033[0;32m Done!ヾ(•ω•`)o \033[0m" << std::endl;

    std::vector<runTAC> runtacs = TAC_to_runTAC(tacs);//将tacs转换为runTAC

    std::clock_t start = clock();
    
    execute(runtacs);

    std::clock_t end   = clock();

    if (extractor.wall_clock)
        std::cout << "Wall clock time:" << (double)(end - start) / CLOCKS_PER_SEC << "s" << std::endl;

    WHUPout_e <<"\033[0;32m Execute success!ヾ(•ω•`)o \033[0m"<<std::endl;
    return 0;
}
