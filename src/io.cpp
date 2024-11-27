#include "whup_io.h"

IO::IO(const std::string &in_file, const std::string &out_file)
{
    in.open(in_file);

    if(!in){
        std::cerr << "\033[31m Error (⊙_⊙)!!! : " << in_file << ": No such file!\033[0m" << std::endl;
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


std::string IO::read_line(int line)
{
	//line行数限制 1 - lines
	std::string line_value;
    
    //行0 - 行lines对应strvect[0] - strvect[lines]
	for (int i=1;i<=line;i++)  
	{
        //读取line次，最终实现读取到line行并将其内容存入line_value
		std::getline(in, line_value, '\n');
	}
	return line_value;
}

