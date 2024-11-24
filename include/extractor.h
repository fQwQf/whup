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
};