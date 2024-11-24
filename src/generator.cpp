#include "generator.h"
#include "whup_parser.h"
#include "function.h"

extern std::vector<ThreeAddressCode> tacs;
extern std::unordered_map<std::string, std::string> var_declares; // 存储将放入c++中变量名和类型的哈希表


/*
代码生成器
读取三地址码和声明表，生成相应的c++代码
*/
std::string generator()
{
    std::string code = "";
    for (auto &i : var_declares)
    {
        if (i.second == "number")
        {
            code += "float " + i.first + ";\n";
        }
        else if (i.second == "string")
        {
            code += "string " + i.first + ";\n";
        }
        else
        {
            code += i.second + " " + i.first + ";\n";
        }
    }

    for (auto &i : tacs)
    {
        if (i.op == "=")
        {
            code += i.result + " = " + i.arg1 + ";\n";
        }
        else if (!std::isdigit(i.op[0]) && !std::isalpha(i.op[0]))
        {
            if (i.op == "%")
            {
                code += i.result + " = fmod(" + i.arg1 + "," + i.arg2 + ");\n";
            }
            else
            {
                code += i.result + " = " + i.arg1 + i.op + i.arg2 + ";\n";
            }
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
        else if (i.op == "print")
        {
            if (i.result == "bool")
            {
                code += "if(" + i.arg1 + ") cout << \"True\" << endl;\n";
                code += "else cout << \"False\" << endl;\n";
            }
            else
            {
                code += "cout << " + i.arg1 + " << endl;\n";
            }
        }
    }

    code = "#include <bits/stdc++.h>\n#ifdef _WIN32\n#include \"windows.h\"\n#endif\nusing namespace std;\nint main()\n{\n#ifdef _WIN32\nSetConsoleOutputCP(CP_UTF8);\n#endif\n" + code + "\nend_of_file:\nreturn 0;\n\n}";

    return code;
}
