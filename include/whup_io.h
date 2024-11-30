#include <bits/stdc++.h>
#include "whup_parser.h"


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

    std::string read_line(int line);

    void writeTAC(const std::vector<ThreeAddressCode>& tacs);

    std::string escapeQuotes(const std::string& str);

    std::vector<std::string> splitHUSTLine(const std::string& line);

    std::vector<ThreeAddressCode> readTAC();

    std::string opTACtoHUST(const std::string& op);

    std::string opHUSTtoTAC(const std::string& op);
    
};