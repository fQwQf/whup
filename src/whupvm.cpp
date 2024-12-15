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

bool print_c1 = false;
bool print_c2 = false;
bool print_e = false;

extern WHUPstream_compile2 WHUPout_c2;
extern WHUPstream_execute WHUPout_e;


int main(int n, const char *arg[])
{
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif

    if(n==1){
        std::cout << "Usage: whupc <input_file> | -i <input_file> | -o <output_file>" << std::endl;
        return 0;
    }


    Extractor extractor(n, arg);//提取参数


    //确定是否打印信息
    print_c2 = extractor.print_c2;
    print_e = extractor.print_e;

    //用io类读取hust文件内容
    IO io(extractor.get_input_file());
    io.readTAC();

    std::vector<runTAC> runtacs = TAC_to_runTAC(tacs);//将tacs转换为runTAC

    std::clock_t start = clock();
    
    execute(runtacs);

    std::clock_t end = clock();

    if (extractor.wall_clock)
        std::cout << "Wall clock time:" << (double)(end - start) / CLOCKS_PER_SEC << "s" << std::endl;

    WHUPout_e <<"Execute success!"<<std::endl;
    return 0;
}
