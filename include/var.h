#ifndef PARSER_H_
#include"whup_parser.h"
#endif

//用来声明变量的类
class Var{
    Environment* env;
    std::string type;
    std::string name;
    std::string value;
    
    //“声明”函数，，将新声明的变量加入符号表中
    void declare();

    public:
    Var(std::vector<Token> tokens,Environment* env);

    //分别是有类型和没有类型的变量声明
    void var(Environment* env, std::string type, std::string name);
    void var(Environment* env, std::string name);
};
