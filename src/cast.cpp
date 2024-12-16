#include "cast.h"
#include "check.h"

extern std::vector<ThreeAddressCode> tacs; // 存储三地址代码的向量

Cast::Cast(std::vector<Token>& expr,Environment* env){
    type=expr[4].value;
    input = expr[2].value;
    output = expr[6].value;

    std::cout << "Cast: " << type << " " << input << " " << output << std::endl;

    if(type=="string"){
        tacs.push_back({NTOS,env->get_var(input),"",env->get_var(output)});
    }else if(type=="number"){
        tacs.push_back({STON,env->get_var(input),"",env->get_var(output)});
    }else{
        //TODO 报错(OK)
        pushErrors(expr[0], "Type Error");//类型错误
    }
}