#ifndef PARSER_H_
#include"whup_parser.h"
#endif
#ifndef OBJECT_H_
#define OBJECT_H_
enum LimitsType{PRIVATE,PROTECTED,PUBLIC};
class Object
{
    private:
    Environment*env;
    public:
    //以分号为分隔扫描,另外，注意对private，public，protected的处理
    Object(std::vector<Token>tokens);

    //处理block (按分号;)
    void addressBlock(std::vector<Token>tokens,LimitsType type);
    //变量声明
    void generateVar(std::vector<Token> subtokens,LimitsType type);
    //声明函数
    void generateFunction(std::vector<Token> subtokens,LimitsType type) ;

    void matchBrace(int&i,std::vector<Token>&tokens);
};
#endif