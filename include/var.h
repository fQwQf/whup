#ifndef PARSER_H_
#include"whup_parser.h"
#endif

//用来声明变量的类
class Var{
    Environment* env;
    std::string type;
    std::string name;
    std::string value;
    void declare();

    public:
    Var(std::vector<Token> tokens,Environment* env);

    //分别是有类型和没有类型的变量声明
    void var(Environment* env, std::string type, std::string name);
    void var(Environment* env, std::string name);
};
