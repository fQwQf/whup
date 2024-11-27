#include"whup_parser.h"
#include<variant>

extern std::vector<ThreeAddressCode>tacs;//全局的三地址码
extern std::unordered_map<std::string,std::string>var_declares;

std::unordered_map<std::string,float>runtimeEnv_number;//
std::unordered_map<std::string,std::string>runtimeEnv_string;//
std::unordered_map<std::string,int>labelMap;//存label与行数的对应关系

bool isString(ThreeAddressCode&tac)
{
    if(var_declares[tac.arg1]=="string"||var_declares[tac.arg2]=="string")
    {
        return true;
    }
    else
    return false;
}


void execute(std::vector<ThreeAddressCode>tacs)
{
    for(int i=0;i<tacs.size();i++)
    {
        ThreeAddressCode tac=tacs[i];//一方面用临时变量更清晰，另一方面用索引记录行数
        
        
        
    }
}