#include <bits/stdc++.h>

//用于从传入参数中提取所需要的信息
class Extractor
{
    std::string input_file;
    std::string output_file;

    public:
        Extractor(int n,const char* arg[]);

        std::string get_input_file();

        std::string get_output_file();
};