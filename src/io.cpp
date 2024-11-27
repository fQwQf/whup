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
	for (int i=1;i<=line;i++)  //行0 - 行lines对应strvect[0] - strvect[lines]
	{
		std::getline(in, line_value, '\n');
	}
	return line_value;
}

void IO::writeTAC(const std::vector<ThreeAddressCode>& tacs){

    for (const auto& tac : tacs) {
        out << "\"" << escapeQuotes(tac.op) << "\",\"" << escapeQuotes(tac.arg1) << "\",\"" << escapeQuotes(tac.arg2) << "\",\"" << escapeQuotes(tac.result) << "\"\n";
    }

}

std::string IO::escapeQuotes(const std::string& str) {
    std::string result;
    for (char ch : str) {
        if (ch == '\"') {
            result += "\\\"";
        } else {
            result += ch;
        }
    }
    return result;
}

std::vector<std::string> IO::splitCSVLine(const std::string& line) {
    std::vector<std::string> result;
    std::stringstream ss(line);
    std::string cell;
    bool inQuotes = false;
    for (size_t i = 0; i < line.size(); ++i) {
        char ch = line[i];
        if (ch == '\\') {
            if (i + 1 < line.size() && line[i + 1] == '\"') {
                // 如果下一个字符是引号，则这是一个转义的引号
                cell += line[i + 1];
                ++i; // 跳过下一个引号
            } else {
                cell += ch;
            }
        } else if(ch == '\"'){
            inQuotes = !inQuotes;
        }
        else if (ch == ',' && !inQuotes) {
            result.push_back(cell);
            cell.clear();
        } else {
            cell += ch;
        }
    }
    if (!cell.empty() || inQuotes) {
        result.push_back(cell);
    }
    return result;
}

std::vector<ThreeAddressCode> IO::readTAC() {
    std::vector<ThreeAddressCode> tacs;

    std::string line;
    while (std::getline(in, line)) {
        std::vector<std::string> parts = splitCSVLine(line);
        if (parts.size() == 4) {
            ThreeAddressCode tac;
            tac.op = parts[0];
            tac.arg1 = parts[1];
            tac.arg2 = parts[2];
            tac.result = parts[3];
            tacs.push_back(tac);
        }
    }

    return tacs;
}
