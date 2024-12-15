#include "whupinput.h"

extern std::vector<ThreeAddressCode> tacs; // 存储三地址代码的向量

WInput::WInput(std::vector<Token>& expr,Environment* env){
    input = expr[2].value;

    std::cout << "WInput: " << input << std::endl;

    if(env->get_var(input)!="null"){
        tacs.push_back({WINPUT,env->get_var(input),"",""});
    }else{
        //TODO 报错
    }
}
