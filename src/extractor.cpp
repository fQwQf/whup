// 从传入参数中提取所需要的信息
#include "extractor.h"


//参数n表示命令行参数的数量，const char *arg[]表示一个字符串数组，包含命令行参数
Extractor::Extractor(int n,const char *arg[])
{
    for (int i = 1; i < n; i++)
    {
        if (strcmp(arg[i],"-i")==0 || strcmp(arg[i],"--input") == 0)//这里不能直接用==，因为char的设计太智障了
        {
            i++;
            input_file = arg[i];
        }
        else if (strcmp(arg[i],"-o") == 0 || strcmp(arg[i],"--output") == 0)
        {
            i++;
            output_file = arg[i];
        }
        else if (strcmp(arg[i],"-c") == 0 || strcmp(arg[i],"--clock") == 0)
        {
            wall_clock = true;
        }
        else if (strcmp(arg[i],"-pc1") == 0 || strcmp(arg[i],"--print-c1") == 0){
            print_c1 = true;
        }
        else if (strcmp(arg[i],"-pc2") == 0 || strcmp(arg[i],"--print-c2") == 0){
            print_c2 = true;
        }
        else if (strcmp(arg[i],"-pe") == 0 || strcmp(arg[i],"--print-e") == 0){
            print_e = true;
        }
        else//如果参数不是以"-i"或"-o"开头，则默认为input
        {
            input_file = arg[i];
        }
    }
}

std::string Extractor::get_input_file()
{
    return input_file;
}

std::string Extractor::get_output_file()
{
    return output_file;
}
