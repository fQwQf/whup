#include"execute.h"
#include"whup_parser.h"

extern std::vector<ThreeAddressCode>tacs;//全局的三地址码
extern std::unordered_map<std::string,std::string>var_declares;
extern std::string newTempLabel();
std::stack<std::string>labelStack;

std::unordered_map<std::string,float>runtimeEnv_number;//
std::unordered_map<std::string,std::string>runtimeEnv_string;//
std::unordered_map<std::string,int>labelMap;//存label与行数的对应关系
std::stack<float>functionStack_number;
std::stack<std::string>functionStack_string;

bool isString(ThreeAddressCode&tac)
{
    if(var_declares[tac.arg1]=="string"||var_declares[tac.arg2]=="string")
    {
        return true;
    }
    else
    return false;
}
bool isNumberString(std::string s)
{
    if(std::all_of(s.begin(),s.end(),::isdigit))
    {
        return true;
    }
    else
    {
        char*p;
        std::strtod(s.c_str(),&p);
        return *p==0;
    }
}
void setLabel(std::vector<ThreeAddressCode>tacs)
{
    for(int i=0;i<tacs.size();i++)
    {
        if(tacs[i].op=="label")
        {
            labelMap[tacs[i].result]=i;
        }
    }
}

void execute(std::vector<ThreeAddressCode> tacs)
{
    setLabel(tacs);
    for(int i=0;i<tacs.size();i++)
    {
        ThreeAddressCode tac=tacs[i];//一方面用临时变量更清晰，另一方面用索引记录行数
        std::cout<<tac.op<<" "<<tac.arg1<<" "<<tac.arg2<<" "<<tac.result<<std::endl;
        if(tac.op=="=")
        {
            if(isString(tac))
            {
                runtimeEnv_string[tac.result]=runtimeEnv_string[tac.arg1];
            }
            else
            {
                runtimeEnv_number[tac.result]=runtimeEnv_number[tac.arg1];
            }
        }
        else if(tac.op=="+")
        {
            if(isString(tac))
            {
                runtimeEnv_string[tac.result]=runtimeEnv_string[tac.arg1]+runtimeEnv_string[tac.arg2];
            }
            else
            {
                runtimeEnv_number[tac.result]=runtimeEnv_number[tac.arg1]+runtimeEnv_number[tac.arg2];
            }
        }
        else if(tac.op=="-")
        {
            runtimeEnv_number[tac.result]=runtimeEnv_number[tac.arg1]-runtimeEnv_number[tac.arg2];
        }
        else if(tac.op=="*")
        {
            runtimeEnv_number[tac.result]=runtimeEnv_number[tac.arg1]*runtimeEnv_number[tac.arg2];
        }
        else if(tac.op=="/")
        {
            runtimeEnv_number[tac.result]=runtimeEnv_number[tac.arg1]/runtimeEnv_number[tac.arg2];
        }
        else if(tac.op=="%")//???怎么算的？？？
        {
            runtimeEnv_number[tac.result]=runtimeEnv_number[tac.arg1]-runtimeEnv_number[tac.arg2]*int(runtimeEnv_number[tac.arg1]/runtimeEnv_number[tac.arg2]);
        }
        else if(tac.op=="==")
        {
            runtimeEnv_number[tac.result]=runtimeEnv_number[tac.arg1]==runtimeEnv_number[tac.arg2];
        }
        else if(tac.op=="!=")
        {
            runtimeEnv_number[tac.result]=runtimeEnv_number[tac.arg1]!=runtimeEnv_number[tac.arg2];
        }
        else if(tac.op=="<")
        {
            runtimeEnv_number[tac.result]=runtimeEnv_number[tac.arg1]<runtimeEnv_number[tac.arg2];
        }
        else if(tac.op=="<=")
        {
            runtimeEnv_number[tac.result]=runtimeEnv_number[tac.arg1]<=runtimeEnv_number[tac.arg2];
        }
        else if(tac.op==">")
        {
            runtimeEnv_number[tac.result]=runtimeEnv_number[tac.arg1]>runtimeEnv_number[tac.arg2];
        }
        else if(tac.op==">=")
        {
            runtimeEnv_number[tac.result]=runtimeEnv_number[tac.arg1]>=runtimeEnv_number[tac.arg2];
        }
        else if(tac.op=="||")
        {
            runtimeEnv_number[tac.result]=runtimeEnv_number[tac.arg1]||runtimeEnv_number[tac.arg2];
        }
        else if(tac.op=="&&")
        {
            runtimeEnv_number[tac.result]=runtimeEnv_number[tac.arg1]&&runtimeEnv_number[tac.arg2];
        }
        // else if(tac.op=="label")//label什么也不干，只是记录自己的索引
        // {
        //     labelMap[tac.result]=i;
        // }
        else if(tac.op=="goto")
        {
            i=labelMap[tac.result];
        }
        else if(tac.op=="if_goto")
        {
            if(tac.result=="end_of_file")
            {
                return;
            }
            if(runtimeEnv_number[tac.arg1])
            {
                i=labelMap[tac.result];
            }
        }
        else if(tac.op=="print")
        {
            if(var_declares[tac.arg1]=="string")
            {
                std::cout<<runtimeEnv_string[tac.arg1]<<std::endl;
            }
            else
            {
                std::cout<<runtimeEnv_number[tac.arg1]<<std::endl;
            }
        }
        else if(tac.op=="input")
        {
            if(var_declares[tac.arg1]=="string")
            {
                std::cin>>runtimeEnv_string[tac.arg1];
            }
            else
            {
                std::cin>>runtimeEnv_number[tac.arg1];
            }
        }
        else if(tac.op=="push")
        {
            if(isString(tac))
            {
                std::string strPara=runtimeEnv_string[tac.arg1];
                functionStack_string.push(strPara);
            }
            else
            {
                float floatPara=runtimeEnv_number[tac.arg1];
                functionStack_number.push(floatPara);
            }
        }
        else if(tac.op=="pop")
        {
            if(isString(tac))
            {
                runtimeEnv_string[tac.result]=functionStack_string.top();
                functionStack_string.pop();
            }
            else
            {
                runtimeEnv_number[tac.result]=functionStack_number.top();
                functionStack_number.pop();
            }
        }
        else if(tac.op=="call")
        {
            std::string temp=newTempLabel();
            labelMap[temp]=i;
            labelStack.push(temp);
            i=labelMap[tac.arg1];
        }
        else if(tac.op=="return")
        {
            i=labelMap[labelStack.top()];
            labelStack.pop();
        }
        else if(tac.op=="end_of_file")
        {
            return;
        }
    }
}