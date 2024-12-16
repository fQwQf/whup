#include "whupinput.h"
#include "check.h"

extern std::vector<ThreeAddressCode> tacs; // 存储三地址代码的向量

WInput::WInput(std::vector<Token>& expr,Environment* env){
    input = expr[2].value;

    std::cout << "WInput: " << input << std::endl;

    if(env->get_type_var(input)=="string"){
        tacs.push_back({WINPUT,env->get_var(input),"",""});
    }else{
        //TODO 报错(OK)
        pushErrors(expr[0], "Type Error");//类型错误
    }
}
