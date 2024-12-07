#include"execute.h"
#include"whup_parser.h"

extern std::vector<ThreeAddressCode>tacs;//全局的三地址码
extern std::unordered_map<std::string,std::string>var_declares;
extern std::string newTempLabel();
std::stack<int>labelStack;

std::unordered_map<std::string,float>runtimeEnv_number;//
std::unordered_map<std::string,std::string>runtimeEnv_string;//
std::unordered_map<std::string,int>labelMap;//存label与行数的对应关系
std::stack<float>functionStack_number;
std::stack<std::string>functionStack_string;

//std::vector<runTAC> runtimeTACs;// 存储运行时三地址码
std::unordered_map<std::string,float*>runtime_number;//
std::unordered_map<std::string,std::string*>runtime_string;//

struct runTAC{
    Operator opperator;       // 操作符
    void* arg1;   // 变量1指针
    void* arg2;   // 变量2指针
    void* result; // 存储结果的变量指针
    int line; // 存储跳转的行号，只在goto里用到
};

std::vector<runTAC> TAC_to_runTAC(std::vector<ThreeAddressCode>tacs){
    setLabel(tacs);

    std::vector<runTAC> runtimeTACs(tacs.size());
    for (auto i : runtimeEnv_number){
        runtime_number[i.first]=&i.second;
    }

    for (auto i : runtimeEnv_string){
        runtime_string[i.first]=&i.second;
    }

    for(int i=0;i<tacs.size();i++)
    {
        ThreeAddressCode tac=tacs[i];//一方面用临时变量更清晰，另一方面用索引记录行数
        // std::cout<<tac.op<<" "<<tac.arg1<<" "<<tac.arg2<<" "<<tac.result<<std::endl;
        if(tac.opperator==ASSIGN)
        {
            runtimeTACs[i].opperator=ASSIGN;
            runtimeTACs[i].arg1=runtime_number[tac.arg1];
            runtimeTACs[i].arg2=NULL;
            runtimeTACs[i].result=runtime_number[tac.result];
        }
        else if(tac.opperator==STRASSIGN)
        {
            runtimeTACs[i].opperator=STRASSIGN;
            runtimeTACs[i].arg1=runtime_string[tac.arg1];
            runtimeTACs[i].arg2=NULL;
            runtimeTACs[i].result=runtime_string[tac.result];
        }
        else if(tac.opperator==ADD||SUB||MUL||DIV||MOD||POW||EQ||NEQ||GT||GE||LT||LE||AND||OR||NOT)
        {
            runtimeTACs[i].opperator=tac.opperator;
            runtimeTACs[i].arg1=runtime_number[tac.arg1];
            runtimeTACs[i].arg2=runtime_number[tac.arg2];
            runtimeTACs[i].result=runtime_number[tac.result];
        }
        else if(tac.opperator==STRADD)
        {
            runtimeTACs[i].opperator=STRADD;
            runtimeTACs[i].arg1=runtime_string[tac.arg1];
            runtimeTACs[i].arg2=runtime_string[tac.arg2];
            runtimeTACs[i].result=runtime_string[tac.result];
        }
        // else if(tac.op=="label")//label什么也不干，只是记录自己的索引
        // {
        //     labelMap[tac.result]=i;
        // }
        else if(tac.opperator==GOTO)
        {
            runtimeTACs[i].opperator=GOTO;
            runtimeTACs[i].line=labelMap[tac.result];
        }
        else if(tac.opperator==IF_GOTO)
        {
            runtimeTACs[i].opperator=IF_GOTO;
            runtimeTACs[i].arg1=runtime_number[tac.arg1];
            runtimeTACs[i].line=labelMap[tac.result];
        }
        else if(tac.opperator==PRINT)
        {
            runtimeTACs[i].opperator=PRINT;
            if(var_declares[tac.arg1]=="string"||(tac.arg1[0]=='\"'&&*(tac.arg1.end()-1)=='\"'))
            {
                runtimeTACs[i].arg1=runtime_string[tac.arg1];
            }
            else
            {
                runtimeTACs[i].arg1=runtime_number[tac.arg1];
            }
        }
        else if(tac.opperator==WINPUT)
        {
            runtimeTACs[i].opperator=WINPUT;
            if(var_declares[tac.arg1]=="string")
            {
                runtimeTACs[i].arg1=runtime_string[tac.arg1];
            }
            else
            {
                runtimeTACs[i].arg1=runtime_number[tac.arg1];
            }
        }
        else if(tac.opperator==PUSH)
        {
            runtimeTACs[i].opperator=PUSH;
            if(isString(tac))
            {
                runtimeTACs[i].arg1=runtime_string[tac.arg1];
            }
            else
            {
                runtimeTACs[i].arg1=runtime_number[tac.arg1];
            }
        }
        else if(tac.opperator==POP)
        {
            runtimeTACs[i].opperator=POP;
            if(isString(tac))
            {
                runtimeTACs[i].result=runtime_string[tac.result];
            }
            else
            {
                runtimeTACs[i].result=runtime_number[tac.result];
            }
        }
        else if(tac.opperator==CALL)
        {
            runtimeTACs[i].opperator=CALL;
            runtimeTACs[i].line=labelMap[tac.arg1];

        }
        else if(tac.opperator==RET)
        {
            runtimeTACs[i].opperator=RET;
        }
        else if(tac.opperator==EXIT)
        {
            runtimeTACs[i].opperator=EXIT;
        }
    }
}

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

void execute(std::vector<runTAC> runtacs)
{
    
    for(int i=0;i<runtacs.size();i++)
    {
        runTAC tac=runtacs[i];//一方面用临时变量更清晰，另一方面用索引记录行数
        // std::cout<<tac.op<<" "<<tac.arg1<<" "<<tac.arg2<<" "<<tac.result<<std::endl;
        if(tac.opperator==ASSIGN)
        {
            *(float*)tac.result=*(float*)tac.arg1;


            // //每一次都要判断感觉有点丑陋啊。。。//但是就这样吧，能跑就行（doge
            // if(isString(tac)||(tac.arg1[0]=='\"'&&*(tac.arg1.end()-1)=='\"'))
            // {
            //     *(std::string*)tac.result=*(std::string*)tac.arg1;
            // }
            // else
            // {
            //     *(float*)tac.result=*(float*)tac.arg1;
            // }
        }
        else if(tac.opperator==STRASSIGN)
        {
            *(std::string*)tac.result=*(std::string*)tac.arg1;
        }
        else if(tac.opperator==ADD)
        {
            *(float*)tac.result=*(float*)tac.arg1+*(float*)tac.arg2;
            // if(isString(tac))
            // {
            //     *(std::string*)tac.result=*(std::string*)tac.arg1+*(std::string*)tac.arg2;
            // }
            // else
            // {
            //     *(float*)tac.result=*(float*)tac.arg1+*(float*)tac.arg2;
            // }
        }
        else if(tac.opperator==STRADD)
        {
            *(std::string*)tac.result=*(std::string*)tac.arg1+*(std::string*)tac.arg2;
        }
        else if(tac.opperator==SUB)
        {
            *(float*)tac.result=*(float*)tac.arg1-*(float*)tac.arg2;
        }
        else if(tac.opperator==MUL)
        {
            *(float*)tac.result=*(float*)tac.arg1**(float*)tac.arg2;
        }
        else if(tac.opperator==DIV)
        {
            *(float*)tac.result=*(float*)tac.arg1 / *(float*)tac.arg2;
        }
        else if(tac.opperator==MOD)//???怎么算的？？？
        {
            *(float*)tac.result=std::fmod(*(float*)tac.arg1,*(float*)tac.arg2);
        }
        else if(tac.opperator==POW)
        {
            *(float*)tac.result=std::pow(*(float*)tac.arg1,*(float*)tac.arg2);
        }
        else if(tac.opperator==EQ)
        {
            *(float*)tac.result=*(float*)tac.arg1==*(float*)tac.arg2;
        }
        else if(tac.opperator==NEQ)
        {
            *(float*)tac.result=*(float*)tac.arg1!=*(float*)tac.arg2;
        }
        else if(tac.opperator==LT)
        {
            *(float*)tac.result=*(float*)tac.arg1<*(float*)tac.arg2;
        }
        else if(tac.opperator==LE)
        {
            *(float*)tac.result=*(float*)tac.arg1<=*(float*)tac.arg2;
        }
        else if(tac.opperator==GT)
        {
            *(float*)tac.result=*(float*)tac.arg1>*(float*)tac.arg2;
        }
        else if(tac.opperator==GE)
        {
            *(float*)tac.result=*(float*)tac.arg1>=*(float*)tac.arg2;
        }
        else if(tac.opperator==OR)
        {
            *(float*)tac.result=*(float*)tac.arg1||*(float*)tac.arg2;
        }
        else if(tac.opperator==AND)
        {
            *(float*)tac.result=*(float*)tac.arg1&&*(float*)tac.arg2;
        }
        // else if(tac.op=="label")//label什么也不干，只是记录自己的索引
        // {
        //     labelMap[tac.result]=i;
        // }
        else if(tac.opperator==GOTO)
        {
            i=tac.line;
        }
        else if(tac.opperator==IF_GOTO)
        {
            if(tac.result=="end_of_file")
            {
                return;
            }
            if(*(float*)tac.arg1)
            {
                i=tac.line;
            }
        }
        else if(tac.opperator==PRINT)
        {
            if(var_declares[tacs[i].arg1]=="string"||(tacs[i].arg1[0]=='\"'&&*(tacs[i].arg1.end()-1)=='\"'))
            {
                std::cout<<*(std::string*)tac.arg1<<std::endl;
            }
            else
            {
                std::cout<<*(float*)tac.arg1<<std::endl;
            }
        }
        else if(tac.opperator==WINPUT)
        {
            if(var_declares[tacs[i].arg1]=="string")
            {
                std::cin>>*(std::string*)tac.arg1;
            }
            else
            {
                std::cin>>*(float*)tac.arg1;
            }
        }
        else if(tac.opperator==PUSH)
        {
            if(functionStack_string.size() >= 10000 || functionStack_number.size() >= 10000){
                std::cerr << "\033[31m Runtime Error (⊙ _⊙ )!!! : stack overflow!" << "\033[0m" << std::endl;

                exit(1);
            }

            if(isString(tacs[i]))
            {
                std::string strPara=*(std::string*)tac.arg1;
                functionStack_string.push(strPara);
            }
            else
            {
                float floatPara=*(float*)tac.arg1;
                functionStack_number.push(floatPara);
            }
        }
        else if(tac.opperator==POP)
        {
            if(isString(tacs[i]))
            {
                *(std::string*)tac.result=functionStack_string.top();
                functionStack_string.pop();
            }
            else
            {
                *(float*)tac.result=functionStack_number.top();
                functionStack_number.pop();
            }
        }
        else if(tac.opperator==CALL)
        {
            labelStack.push(i);
            i=tac.line;
        }
        else if(tac.opperator==RET)
        {
            i=labelStack.top();
            labelStack.pop();
        }
        else if(tac.opperator==EXIT)
        {
            return;
        }
    }
}