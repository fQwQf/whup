#include "whup_parser.h"
#include "expression.h"

extern std::vector<ThreeAddressCode> tacs; // 存储三地址代码的向量

//接收一个含有Token的vector，作为expr的输入，输出该expr的结果
class Print{

    public:
        Print(std::vector<Token> &tokens,Environment* env);
};