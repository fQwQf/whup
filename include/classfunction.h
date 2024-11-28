#ifndef BLOCK_H_
#include"block.h"
#endif

#ifndef PARSER_H_
#include"whup_parser.h"
#endif
#ifndef CLASSFUNCTION_H_
#define CLASSFUNCTION_H_

#include "function.h"



//基本上是对function的重复，只是多了一个Object_env并插入新的实例表
class ClassFunction : public Function
{


    public:

        //由于是类函数，所以需要传入类函数表
        //应该需要传引用，不然原来的函数表不会插入
        ClassFunction(std::vector<Token> &tokens,Environment* env,std::unordered_map<std::string,ClassFunction*>& function_table);
        std::string call(std::vector<Token> &tokens,Environment* env);
        
};
#endif