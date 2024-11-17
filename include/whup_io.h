#include <bits/stdc++.h>


//创建一个主管文件读取和写入的类
class IO {
    std::ifstream in;
    std::ofstream out;
public:
    IO(const std::string &in_file, const std::string &out_file);

    ~IO();

    std::string read();

    void write(std::string output);
    
};