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
        // std::cout<<tac.op<<" "<<tac.arg1<<" "<<tac.arg2<<" "<<tac.result<<std::endl;
        if(tac.opperator==ASSIGN)
        {
            runtimeEnv_number[tac.result]=runtimeEnv_number[tac.arg1];


            // //每一次都要判断感觉有点丑陋啊。。。//但是就这样吧，能跑就行（doge
            // if(isString(tac)||(tac.arg1[0]=='\"'&&*(tac.arg1.end()-1)=='\"'))
            // {
            //     runtimeEnv_string[tac.result]=runtimeEnv_string[tac.arg1];
            // }
            // else
            // {
            //     runtimeEnv_number[tac.result]=runtimeEnv_number[tac.arg1];
            // }
        }
        else if(tac.opperator==STRASSIGN)
        {
            runtimeEnv_string[tac.result]=runtimeEnv_string[tac.arg1];
        }
        else if(tac.opperator==ADD)
        {
            runtimeEnv_number[tac.result]=runtimeEnv_number[tac.arg1]+runtimeEnv_number[tac.arg2];
            // if(isString(tac))
            // {
            //     runtimeEnv_string[tac.result]=runtimeEnv_string[tac.arg1]+runtimeEnv_string[tac.arg2];
            // }
            // else
            // {
            //     runtimeEnv_number[tac.result]=runtimeEnv_number[tac.arg1]+runtimeEnv_number[tac.arg2];
            // }
        }
        else if(tac.opperator==STRADD)
        {
            runtimeEnv_string[tac.result]=runtimeEnv_string[tac.arg1]+runtimeEnv_string[tac.arg2];
        }
        else if(tac.opperator==SUB)
        {
            runtimeEnv_number[tac.result]=runtimeEnv_number[tac.arg1]-runtimeEnv_number[tac.arg2];
        }
        else if(tac.opperator==MUL)
        {
            runtimeEnv_number[tac.result]=runtimeEnv_number[tac.arg1]*runtimeEnv_number[tac.arg2];
        }
        else if(tac.opperator==DIV)
        {
            runtimeEnv_number[tac.result]=runtimeEnv_number[tac.arg1]/runtimeEnv_number[tac.arg2];
        }
        else if(tac.opperator==MOD)//???怎么算的？？？
        {
            runtimeEnv_number[tac.result]=std::fmod(runtimeEnv_number[tac.arg1],runtimeEnv_number[tac.arg2]);
        }
        else if(tac.opperator==POW)
        {
            runtimeEnv_number[tac.result]=std::pow(runtimeEnv_number[tac.arg1],runtimeEnv_number[tac.arg2]);
        }
        else if(tac.opperator==EQ)
        {
            runtimeEnv_number[tac.result]=runtimeEnv_number[tac.arg1]==runtimeEnv_number[tac.arg2];
        }
        else if(tac.opperator==NEQ)
        {
            runtimeEnv_number[tac.result]=runtimeEnv_number[tac.arg1]!=runtimeEnv_number[tac.arg2];
        }
        else if(tac.opperator==LT)
        {
            runtimeEnv_number[tac.result]=runtimeEnv_number[tac.arg1]<runtimeEnv_number[tac.arg2];
        }
        else if(tac.opperator==LE)
        {
            runtimeEnv_number[tac.result]=runtimeEnv_number[tac.arg1]<=runtimeEnv_number[tac.arg2];
        }
        else if(tac.opperator==GT)
        {
            runtimeEnv_number[tac.result]=runtimeEnv_number[tac.arg1]>runtimeEnv_number[tac.arg2];
        }
        else if(tac.opperator==GE)
        {
            runtimeEnv_number[tac.result]=runtimeEnv_number[tac.arg1]>=runtimeEnv_number[tac.arg2];
        }
        else if(tac.opperator==OR)
        {
            runtimeEnv_number[tac.result]=runtimeEnv_number[tac.arg1]||runtimeEnv_number[tac.arg2];
        }
        else if(tac.opperator==AND)
        {
            runtimeEnv_number[tac.result]=runtimeEnv_number[tac.arg1]&&runtimeEnv_number[tac.arg2];
        }
        // else if(tac.op=="label")//label什么也不干，只是记录自己的索引
        // {
        //     labelMap[tac.result]=i;
        // }
        else if(tac.opperator==GOTO)
        {
            i=labelMap[tac.result];
        }
        else if(tac.opperator==IF_GOTO)
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
        else if(tac.opperator==PRINT)
        {
            if(var_declares[tac.arg1]=="string"||(tac.arg1[0]=='\"'&&*(tac.arg1.end()-1)=='\"'))
            {
                std::cout<<runtimeEnv_string[tac.arg1]<<std::endl;
            }
            else
            {
                std::cout<<runtimeEnv_number[tac.arg1]<<std::endl;
            }
        }
        else if(tac.opperator==WINPUT)
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
        else if(tac.opperator==PUSH)
        {
            if(functionStack_string.size() >= 10000 || functionStack_number.size() >= 10000){
                std::cerr << "\033[31m Runtime Error (⊙ _⊙ )!!! : stack overflow!" << "\033[0m" << std::endl;

                exit(1);
            }

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
        else if(tac.opperator==POP)
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
        else if(tac.opperator==CALL)
        {
            std::string temp=newTempLabel();
            labelMap[temp]=i;
            labelStack.push(temp);
            i=labelMap[tac.arg1];
        }
        else if(tac.opperator==RET)
        {
            i=labelMap[labelStack.top()];
            labelStack.pop();
        }
        else if(tac.opperator==EXIT)
        {
            return;
        }
    }
}