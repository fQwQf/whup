#ifndef BLOCK_H_
#include"block.h"
#endif

#ifndef PARSER_H_
#include"whup_parser.h"
#endif
#ifndef CLASSFUNCTION_H_
#define CLASSFUNCTION_H_
//基本上是对function的重复，只是多了一个Object_env并插入新的实例表
class ClassFunction
{
    std::string name;
    std::string return_type;
    
    std::string body;
    std::vector<std::pair<std::string,std::string>> params_name;//形参名,分别为原形参名和生成的专用形参名
    std::vector<std::string> params_type;
    std::string end_label; //函数结束标签
    std::string start_label; //函数开始标签

    std::vector<Token> body_tokens;//函数体

    std::vector<std::string> return_labels; //返回时需要跳转回去的标签

    std::string return_value; //储存返回值的临时变量

    std::string jump_in_label;//储存储存跳入标签的临时变量的变量

    Environment* ClassFunction_env;//必然是全局的

    public:
        //由于是类函数，所以需要传入类函数表
        //应该需要传引用，不然原来的函数表不会插入
        ClassFunction(std::vector<Token> &tokens,Environment* env,std::unordered_map<std::string,ClassFunction*>& function_table);
        std::string call(std::vector<Token> &tokens,Environment* env);
        void matchPar(int &i,std::vector<Token> &tokens);
        void generate();//生成函数代码
        std::string get_return_value();
};
#endif