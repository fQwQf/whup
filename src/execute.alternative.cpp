//用函数指针数组的实现，漂亮得很，但真运行起来就不行了
#include "execute.h"
#include "whup_parser.h"

extern std::vector<ThreeAddressCode> tacs; // 全局的三地址码
extern std::unordered_map<std::string, std::string> var_declares;
extern std::string newTempLabel();
std::stack<std::string> labelStack;

std::unordered_map<std::string, float> runtimeEnv_number;       //
std::unordered_map<std::string, std::string> runtimeEnv_string; //
std::unordered_map<std::string, int> labelMap;                  // 存label与行数的对应关系
std::stack<float> functionStack_number;
std::stack<std::string> functionStack_string;

int i=0;

bool isString(ThreeAddressCode &tac)
{
    if (var_declares[tac.arg1] == "string" || var_declares[tac.arg2] == "string")
    {
        return true;
    }
    else
        return false;
}
bool isNumberString(std::string s)
{
    if (std::all_of(s.begin(), s.end(), ::isdigit))
    {
        return true;
    }
    else
    {
        char *p;
        std::strtod(s.c_str(), &p);
        return *p == 0;
    }
}
void setLabel(std::vector<ThreeAddressCode> tacs)
{
    for (int i = 0; i < tacs.size(); i++)
    {
        if (tacs[i].op == "label")
        {
            labelMap[tacs[i].result] = i;
        }
    }
}



void assignOperation(ThreeAddressCode &tac)
{
    runtimeEnv_number[tac.result] = runtimeEnv_number[tac.arg1];

    // //每一次都要判断感觉有点丑陋啊。。。//但是就这样吧，能跑就行（doge
    // if(isString(tac)||(tac.arg1[0]=='\"'&&*(tac.arg1.end()-1)=='\"'))
    // {
    //     runtimeEnv_string[tac.result]=runtimeEnv_string[tac.arg1];
    // }
    // else
    // {
    //     runtimeEnv_number[tac.result]=runtimeEnv_number[tac.arg1];
    // }
};
void strassignOperation(ThreeAddressCode &tac)
{
    runtimeEnv_string[tac.result] = runtimeEnv_string[tac.arg1];
}
void addOperation(ThreeAddressCode &tac)
{
    runtimeEnv_number[tac.result] = runtimeEnv_number[tac.arg1] + runtimeEnv_number[tac.arg2];
    // if(isString(tac))
    // {
    //     runtimeEnv_string[tac.result]=runtimeEnv_string[tac.arg1]+runtimeEnv_string[tac.arg2];
    // }
    // else
    // {
    //     runtimeEnv_number[tac.result]=runtimeEnv_number[tac.arg1]+runtimeEnv_number[tac.arg2];
    // }
}
void straddOperation(ThreeAddressCode &tac)
{
    runtimeEnv_string[tac.result] = runtimeEnv_string[tac.arg1] + runtimeEnv_string[tac.arg2];
}
void subOperation(ThreeAddressCode &tac)
{
    runtimeEnv_number[tac.result] = runtimeEnv_number[tac.arg1] - runtimeEnv_number[tac.arg2];
}
void mulOperation(ThreeAddressCode &tac)
{
    runtimeEnv_number[tac.result] = runtimeEnv_number[tac.arg1] * runtimeEnv_number[tac.arg2];
}
void divOperation(ThreeAddressCode &tac)
{
    runtimeEnv_number[tac.result] = runtimeEnv_number[tac.arg1] / runtimeEnv_number[tac.arg2];
}
void modOperation(ThreeAddressCode &tac) //???怎么算的？？？
{
    runtimeEnv_number[tac.result] = std::fmod(runtimeEnv_number[tac.arg1], runtimeEnv_number[tac.arg2]);
}
void eqOperation(ThreeAddressCode &tac)
{
    runtimeEnv_number[tac.result] = runtimeEnv_number[tac.arg1] == runtimeEnv_number[tac.arg2];
}
void neqOperation(ThreeAddressCode &tac)
{
    runtimeEnv_number[tac.result] = runtimeEnv_number[tac.arg1] != runtimeEnv_number[tac.arg2];
}
void ltOperation(ThreeAddressCode &tac)
{
    runtimeEnv_number[tac.result] = runtimeEnv_number[tac.arg1] < runtimeEnv_number[tac.arg2];
}
void leOperation(ThreeAddressCode &tac)
{
    runtimeEnv_number[tac.result] = runtimeEnv_number[tac.arg1] <= runtimeEnv_number[tac.arg2];
}
void gtOperation(ThreeAddressCode &tac)
{
    runtimeEnv_number[tac.result] = runtimeEnv_number[tac.arg1] > runtimeEnv_number[tac.arg2];
}
void geOperation(ThreeAddressCode &tac)
{
    runtimeEnv_number[tac.result] = runtimeEnv_number[tac.arg1] >= runtimeEnv_number[tac.arg2];
}
void orOperation(ThreeAddressCode &tac)
{
    runtimeEnv_number[tac.result] = runtimeEnv_number[tac.arg1] || runtimeEnv_number[tac.arg2];
}
void andOperation(ThreeAddressCode &tac)
{
    runtimeEnv_number[tac.result] = runtimeEnv_number[tac.arg1] && runtimeEnv_number[tac.arg2];
}
// else if(tac.op=="label")//label什么也不干，只是记录自己的索引
// {
//     labelMap[tac.result]=i;
// }
void gotoOperation(ThreeAddressCode &tac)
{
    i = labelMap[tac.result];
}
void if_gotoOperation(ThreeAddressCode &tac)
{
    if (tac.result == "end_of_file")
    {
        return;
    }
    if (runtimeEnv_number[tac.arg1])
    {
        i = labelMap[tac.result];
    }
}
void printOperation(ThreeAddressCode &tac)
{
    if (var_declares[tac.arg1] == "string" || (tac.arg1[0] == '\"' && *(tac.arg1.end() - 1) == '\"'))
    {
        std::cout << runtimeEnv_string[tac.arg1] << std::endl;
    }
    else
    {
        std::cout << runtimeEnv_number[tac.arg1] << std::endl;
    }
}
void winputOperation(ThreeAddressCode &tac)
{
    if (var_declares[tac.arg1] == "string")
    {
        std::cin >> runtimeEnv_string[tac.arg1];
    }
    else
    {
        std::cin >> runtimeEnv_number[tac.arg1];
    }
}
void pushOperation(ThreeAddressCode &tac)
{
    if (functionStack_string.size() >= 100000000 || functionStack_number.size() >= 1000000000)
    {
        std::cerr << "\033[31m Runtime Error (⊙ _⊙ )!!! : stack overflow!" << "\033[0m" << std::endl;

        exit(1);
    }

    if (isString(tac))
    {
        std::string strPara = runtimeEnv_string[tac.arg1];
        functionStack_string.push(strPara);
    }
    else
    {
        float floatPara = runtimeEnv_number[tac.arg1];
        functionStack_number.push(floatPara);
    }
}
void popOperation(ThreeAddressCode &tac)
{
    if (isString(tac))
    {
        runtimeEnv_string[tac.result] = functionStack_string.top();
        functionStack_string.pop();
    }
    else
    {
        runtimeEnv_number[tac.result] = functionStack_number.top();
        functionStack_number.pop();
    }
}
void callOperation(ThreeAddressCode &tac)
{
    std::string temp = newTempLabel();
    labelMap[temp] = i;
    labelStack.push(temp);
    i = labelMap[tac.arg1];
}
void retOperation(ThreeAddressCode &tac)
{
    i = labelMap[labelStack.top()];
    labelStack.pop();
}


void notOperation(ThreeAddressCode &tac){
    return;
}

void labelOperation(ThreeAddressCode &tac)
{
    return;
}

void exitOperation(ThreeAddressCode &tac)
{
    i=tacs.size();
}




void (*OperationFunc[])(ThreeAddressCode &tac) =
    {
        addOperation,       // +
        straddOperation,    // 字符串相加
        subOperation,       // -
        mulOperation,       // *
        divOperation,       // /
        modOperation,       // %
        andOperation,       // &&
        orOperation,        // ||
        notOperation,       // !
        eqOperation,        // ==
        neqOperation,       // !=
        ltOperation,        // <
        gtOperation,        // >
        leOperation,        // <=
        geOperation,        // >=
        assignOperation,    // =
        strassignOperation, // 字符串赋值
        labelOperation,     // label
        gotoOperation,      // goto
        if_gotoOperation,   // if_goto
        printOperation,     // print
        winputOperation,    // input
        pushOperation,      // push
        popOperation,       // pop
        callOperation,      // call
        retOperation,       // return
        exitOperation       // end
};


void execute(std::vector<ThreeAddressCode> tacs)
{
    setLabel(tacs);
    for (; i < tacs.size(); i++)
    {
        ThreeAddressCode tac = tacs[i]; // 一方面用临时变量更清晰，另一方面用索引记录行数
        OperationFunc[tac.opperator](tac);
        // std::cout<<tac.op<<" "<<tac.arg1<<" "<<tac.arg2<<" "<<tac.result<<std::endl;
    }

    return;
}

