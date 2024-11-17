#include "generator.h"
#include "whup_parser.h"

extern std::vector<ThreeAddressCode> tacs;
extern std::unordered_map<std::string, std::string> var_declares;  // 存储将放入c++中变量名和类型的哈希表

/*
生成器
读取三地址码和声明表，生成相应的c++代码
*/

std::string generator()
{
    /*std::string code = "";
    for (auto &i : var_declares)
    {   
        if (i.second == "number"){
            code += "float " + i.first + ";\n";
        }else{
            code += i.second  + " " + i.first + ";\n";
        }
        
    }*/

    /*for (auto &i : tacs)
    {
        if (i.op == "=")
        {
            code += i.result + " = " + i.arg1 + ";\n";
        }
        else if (!std::isdigit(i.op[0]) && !std::isalpha(i.op[0]))
        {
            code += i.result + " = " + i.arg1 + i.op + i.arg2 + ";\n";
        }
        else if (i.op == "label")
        {
            code += i.result + ":\n";
        }
        else if (i.op == "if_goto")
        {
            code += "if(" + i.arg1 + ") goto " + i.result + ";\n";
        }
        else if (i.op == "goto")
        {
            code += "goto " + i.result + ";\n";
        }
    }*/

    return "";//code;
}
