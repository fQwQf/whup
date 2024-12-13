#include<bits/stdc++.h>
#include "whup_parser.h"

extern std::stack<int> labelStack;

struct runTAC{
    Operator opperator;       // 操作符
    void** arg1;   // 变量1指针
    void** arg2;   // 变量2指针
    void** result; // 存储结果的变量指针
    int line; // 存储跳转的行号，只在goto里用到
};

std::vector<runTAC> TAC_to_runTAC(std::vector<ThreeAddressCode> &tacs);

void execute(std::vector<runTAC> runtacs);

