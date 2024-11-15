#include "block.h"
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
    Block(std::vector<Token> tokens, Environment* env){
        this->env = new Environment(env);

        int last_semicolon = 0;

        for(int i = 0; i < tokens.size(); i++){
            if(tokens[i].type == SYMBOL || tokens[i].value == ";"){
                std::vector<Token> subtokens(tokens.begin() + last_semicolon, tokens.begin() + i - 1);
                last_semicolon += 1;
                generate(subtokens);
            }
        }
    }

    //根据首token传入对应的类的构造函数中。
    void generate(std::vector<Token> subtokens){
        if(subtokens[0].type == IDENTIFIER){
            //new Assign(subtokens);
        }else if(subtokens[0].type == KEYWORD && subtokens[0].value == "var"){
            //new Var(subtokens);
        }
    }
    

};