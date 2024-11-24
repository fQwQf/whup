#include "whup_io.h"

IO::IO(const std::string &in_file, const std::string &out_file)
{
    in.open(in_file);

    if(!in){
        std::cout << "\033[0;31m Error(っ °Д °;)っ \033[0m " << in_file << ": no sucn file!" << std::endl;
        exit(1);
    }
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
    while (in >> temp){
        line += temp + " ";
    };
    return line;
}

void IO::write(std::string output)
{
    out << output;
}
