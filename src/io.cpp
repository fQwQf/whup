#include "whup_io.h"

IO::IO(const std::string &in_file, const std::string &out_file)
{
    in.open(in_file);
    out.open(out_file);
}

IO::~IO()
{
    in.close();
    out.close();
}

std::string IO::read()
{
    std::string line,temp;
    char ch;
    //以逐个的字符形式读取文件，能够保留原有格式（包括空格和换行符），便于代码检查
    while (in.get(ch)){
        line += ch;
    };
    return line;
}

void IO::write(std::string output)
{
    out << output;
}
