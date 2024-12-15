#include <bits/stdc++.h>

//用于从传入参数中提取所需要的输入和输出文件路径
class Extractor
{
    std::string input_file;//存储输入文件路径
    std::string output_file;//存储输出文件路径
    

    public:
        Extractor(int n,const char* arg[]);

        std::string get_input_file();

        std::string get_output_file();
        
        bool wall_clock = false;//是否使用wall clock计时器

        bool print_c1 = false;//是否打印第一次编译详细信息
        bool print_c2 = false;//是否打印第二次编译详细信息
        bool print_e = false;//是否打印执行详细信息
};