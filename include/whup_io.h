#include <bits/stdc++.h>


//创建一个主管文件读取和写入的类
//处理文件的输入和输出操作
class IO {
    std::ifstream in;//输入文件流类型
    std::ofstream out;//输出文件流类型
public:
    IO(const std::string &in_file, const std::string &out_file);

    ~IO();

    //从in_file中返回读取内容
    std::string read();

    //将out_file内容写入output
    void write(std::string output);
    
};