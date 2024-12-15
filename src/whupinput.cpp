#include "whupinput.h"
#include "WHUPstream.h"

extern WHUPstream_compile1 WHUPout;

extern std::vector<ThreeAddressCode> tacs; // 存储三地址代码的向量

WInput::WInput(std::vector<Token>& expr,Environment* env){
    input = expr[2].value;

    WHUPout << "WInput: " << input << std::endl;

    if(env->get_type_var(input)=="string"){
        tacs.push_back({WINPUT,env->get_var(input),"",""});
    }else{
        //TODO 报错
    }
}
