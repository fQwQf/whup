#include "cast.h"
#include "WHUPstream.h"

extern WHUPstream_compile1 WHUPout;

extern std::vector<ThreeAddressCode> tacs; // 存储三地址代码的向量

Cast::Cast(std::vector<Token>& expr,Environment* env){
    type=expr[4].value;
    input = expr[2].value;
    output = expr[6].value;

    WHUPout << "Cast: " << type << " " << input << " " << output << std::endl;

    if(type=="string"){
        tacs.push_back({NTOS,env->get_var(input),"",env->get_var(output)});
    }else if(type=="number"){
        tacs.push_back({STON,env->get_var(input),"",env->get_var(output)});
    }else{
        //TODO 报错
    }
}