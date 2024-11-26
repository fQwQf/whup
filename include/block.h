#ifndef PARSER_H_
#include"whup_parser.h"
#endif

#ifndef BLOCK_H_
#define BLOCK_H_
#include <bits/stdc++.h>


/*
*构建block最主要的目的是便于确定变量作用域。
*每个block都要指向一个environment对象。
*每生成一个新的block，都要将当前block的environment对象作为参数传入新block的构造函数中。
自然，构造函数应当传入一个储存Token的vector。
对该vector进行扫描，以分号为分隔，将每个单元根据首Token传入对应的类的构造函数中。
*/
class Block{
    std::vector<ThreeAddressCode> code;

    Environment* env;
public:

    //以分号为分隔扫描
    Block(std::vector<Token> tokens, Environment* env);
    Block(std::vector<Token> tokens);
    void block(std::vector<Token> tokens);

    //跳过大括号
    void matchBrace(int &i,std::vector<Token> &tokens);

    //根据首token传入对应的类的构造函数中。
    void generate(std::vector<Token> subtokens);
    

};
#endif
