#include "return.h"

extern std::string function_ret_label; // 函数返回标签，可用于检测是否在处理函数。
extern std::vector<ThreeAddressCode> tacs; // 存储三地址代码的向量

class Return {

    private:
    Environment *env;

    public:
    Return(Environment *env){
        this->env = env;
        tacs.push_back({"if_goto","1","",function_ret_label}); // 如果当前在处理函数，则跳转到函数返回标签
    }

    Return(std::vector<Token> tokens,Environment *env){
        this->env = env;
        if (tokens.size() != 2) {
            std::cout << "return 语句格式错误" << std::endl;
        }
        else {
            
        }
        tacs.push_back({"return","","",function_ret_label}); // 生成返回语句
    }
};