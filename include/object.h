#ifndef PARSER_H_
#include"whup_parser.h"
#endif
#ifndef OBJECT_H_
#define OBJECT_H_
class Object
{
    private:
    Environment*env;
    public:
    //以分号为分隔扫描,另外，注意对private，public，protected的处理
    Object(std::vector<Token>tokens,Environment*e);

    //根据首token传入对应的类的构造函数中。
    void generate(std::vector<Token> subtokens);
};
#endif