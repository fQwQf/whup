#include "whup_io.h"
#include "WHUPstream.h"

extern WHUPstream_compile1 WHUPout;

//序列化就是要读写这五个
extern std::vector<ThreeAddressCode> tacs;  // 存储三地址代码的向量
extern std::vector<ThreeAddressCode> tacArrs;  
extern std::unordered_map<std::string, std::string> var_declares;//存储变量的声明信息
extern std::unordered_map<std::string,float>runtimeEnv_number;//
extern std::unordered_map<std::string,std::string>runtimeEnv_string;//

IO::IO(const std::string &in_file, const std::string &out_file)
{
    in.open(in_file);

    if(!in){
        std::cerr << "\033[31m Error (⊙_⊙)!!! : " << in_file << ": No such file!\033[0m" << std::endl;
        exit(1);
    }
    out.open(out_file);
}

IO::IO(const std::string &in_file)
{
    in.open(in_file);

    if(!in){
        std::cerr << "\033[31m Error (⊙_⊙)!!! : " << in_file << ": No such file!\033[0m" << std::endl;
        exit(1);
    }
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

void IO::writeTAC(){

    for (const auto& tac : tacs) {
        out << tac.opperator << " " << escapeQuotes(tac.arg1) << " " << escapeQuotes(tac.arg2) << " " << escapeQuotes(tac.result) << "\n";
    }

    WHUPout << "write TAC" << std::endl;

    out << "\n";

    for (const auto& tac : tacArrs) {
        out << tac.opperator << " " << escapeQuotes(tac.arg1) << " " << escapeQuotes(tac.arg2) << " " << escapeQuotes(tac.result) << "\n";
    }

    WHUPout << "write Arr" << std::endl;

    out << "\n";

    for (auto var : var_declares) {
        out << "VAR " << var.first << " " << var.second << "\n";
    }

    WHUPout << "write VAR" << std::endl;
    
    out << "\n";

    for (const auto& [key, value] : runtimeEnv_number) {
        out << "NUMBER " << escapeQuotes(key) << " " << escapeQuotes(std::to_string(value)) << "\n";
    }

    WHUPout << "write NUMBER" << std::endl;

    out << "\n";

    for (const auto& [key, value] : runtimeEnv_string) {
        out << "STRING " << escapeQuotes(key) << " " << escapeQuotes(value) << "\n";
    }

    WHUPout << "write STRING" << std::endl;

    out << "\n";

}


std::string IO::opHUSTtoTAC(const std::string& op){
    if(op == "ADD"){
        return "+";
    }else if(op == "SUB"){
        return "-";
    }else if(op == "MUL"){
        return "*";
    }else if(op == "DIV"){
        return "/";
    }else if(op == "MOD"){
        return "%";
    }else if(op == "EQ"){
        return "==";
    }else if(op == "NEQ"){
        return "!=";
    }else if(op == "LT"){
        return "<";
    }else if(op == "LTE"){
        return "<=";
    }else if(op == "GT"){
        return ">";
    }else if(op == "GTE"){
        return ">=";
    }else if(op == "AND"){
        return "&&";
    }else if(op == "OR"){
        return "||";
    }else if(op == "NOT"){
        return "!";
    }else if(op == "MOV"){
        return "=";
    }else if(op == "LAB"){
        return "label";
    }else if(op == "JMP"){
        return "goto";
    }else if(op == "JNZ"){
        return "if_goto";
    }else if(op == "CALL"){
        return "call";
    }else if(op == "RET"){
        return "return";
    }else if(op == "PUSH"){
        return "push";
    }else if(op == "POP"){
        return "pop";
    }else if(op == "EXIT"){
        return "exit";
    }else{
        return op; // 如果没有匹配到，则返回原字符串
    }
}

std::string IO::opTACtoHUST(const std::string& op){
    if(op == "+"){
        return "ADD";
    }else if(op == "-"){
        return "SUB";
    }else if(op == "*"){
        return "MUL";
    }else if(op == "/"){
        return "DIV";
    }else if(op == "%"){
        return "MOD";
    }else if(op == "=="){
        return "EQ";
    }else if(op == "!="){
        return "NEQ";
    }else if(op == "<"){
        return "LT";
    }else if(op == "<="){
        return "LTE";
    }else if(op == ">"){
        return "GT";
    }else if(op == ">="){
        return "GTE";
    }else if(op == "&&"){
        return "AND";
    }else if(op == "||"){
        return "OR";
    }else if(op == "!"){
        return "NOT";
    }else if(op == "="){
        return "MOV";
    }else if(op == "label"){
        return "LAB";
    }else if(op == "goto"){
        return "JMP";
    }else if(op == "if_goto"){
        return "JNZ";
    }else if(op == "call"){
        return "CALL";
    }else if(op == "return"){
        return "RET";
    }else if(op == "push"){
        return "PUSH";
    }else if(op == "pop"){
        return "POP";
    }else if(op == "exit"){
        return "EXIT";
    }else{
        return op; // 如果没有匹配到，则返回原字符串
    }
}


std::string IO::escapeQuotes(const std::string& str) {
    if(str.empty()){
        return "NULL";
    }
    std::string result;
    bool isString = false; //标记是否在字符串中
    if(str[0] == '\"' && str[str.size()-1] == '\"'){
        isString = true;
    }
    for (char ch : str) {
        if (ch == '\"') {
            result += "\\\"";
        } else {
            result += ch;
        }
    }
    if(isString){
        result.erase(0, 1);
        result.erase(result.size() - 2, 1);
    }

    return result;
}

std::vector<std::string> IO::splitHUSTLine(const std::string& line) {
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
            cell = cell + "\"";
        }
        else if (ch == ' ' && !inQuotes) {
            if(cell == "NULL")
            {
                cell = "";
            }
            result.push_back(cell);
            cell.clear();
        } else {
            cell += ch;
        }
    }
    if (!cell.empty() || inQuotes) {
        if(cell == "NULL")
            {
                cell = "";
            }
        result.push_back(cell);
    }
    return result;
}

void IO::readTAC() {
    std::string line;
    while (std::getline(in, line)) {
        std::vector<std::string> parts = splitHUSTLine(line);
        if (parts.size() == 4) {
            
            Operator part0 = Operator(stoi(parts[0]));

            ThreeAddressCode tac{part0,parts[1],parts[2],parts[3]};

            if (part0 != ARRSET)
            {
                tacs.push_back(tac);
            }
            else
            {
                tacArrs.push_back(tac);
            }
        }
        else if(parts.size() == 3)
        {
            if(parts[0] == "VAR")
            {
                var_declares[parts[1]] = parts[2];
            }else if(parts[0] == "NUMBER")
            {
                runtimeEnv_number[parts[1]] = std::stof(parts[2]);
            }else if(parts[0] == "STRING")
            {
                runtimeEnv_string[parts[1]] = parts[2];
            }
            
        }

    }

}

std::string IO::readWHUPLib()
{
    std::filesystem::path folderPath = "./WHUPLib";

    // 检查文件夹是否存在
    if (!std::filesystem::exists(folderPath) || !std::filesystem::is_directory(folderPath))
    {
        //TODO 统一报错
        std::cerr << "Folder WHUPLib does not exist." << std::endl;
        return "";
    }

    std::string result;

    // 遍历文件夹中的所有文件
    for (const auto &entry : std::filesystem::directory_iterator(folderPath))
    {
        in.open(entry.path().string());
        if (in.is_open()) {
            result += read();
        }else{
            //TODO 统一报错
            std::cerr << "Failed to open file: " << entry.path().string() << std::endl;
        }

    }

    return result;
}