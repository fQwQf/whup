#include "block.h"
#include"assign.h"
#include"var.h"
#include"print.h"
#include"if.h"
#include"while.h"
#include"break.h"
#include"continue.h"
#include "return.h"
#include "function.h"


extern std::string function_ret_label; // 函数返回标签，可用于检测是否在处理函数。
extern std::unordered_map<std::string, Function*> functions;  // 存储函数名和对应的对象指针哈希表


//跳过大括号
void Block::matchBrace(int &i,std::vector<Token> &tokens)
{
    if (tokens[i].value == "{")
    {
        int leftPar = 1;//甚至par都没有改成brace
        int rightPar = 0; // 分别记录已经读取的左大括号右大括号的个数,当相等时即可结束
        while (leftPar != rightPar)
        {
            ++i;
            if (tokens[i].value == "}")
            {
                rightPar++;
            }
            else if (tokens[i].value == "{")
            {
                leftPar++;
            }
            else
                continue;
        }
    }
}

Block::Block(std::vector<Token> tokens, Environment *e)
{
    this->env = new Environment(e);

    block(tokens);
}

Block::Block(std::vector<Token> tokens)
{
    this->env = new Environment();

    block(tokens);
}

// 以分号为分隔扫描
void Block::block(std::vector<Token> tokens)
{
    int last_semicolon = 0;

    for (int i = 0; i < tokens.size(); i++)
    {
        //打印出所有Token
        //debug时可能有用
        //std::cout << tokens[i].value;
        matchBrace(i, tokens);
        if (tokens[i].type == SYMBOL && tokens[i].value == ";")
        {
            std::vector<Token> subtokens(tokens.begin() + last_semicolon, tokens.begin() + i);
            last_semicolon = i+1;
            generate(subtokens);
        }
    }
}

// 根据首token传入对应的类的构造函数中。
void Block::generate(std::vector<Token> subtokens)
{
    if (subtokens.empty())
        return;

    if (subtokens[0].type == IDENTIFIER && subtokens[1].value != "(")
    {
        new Assign(subtokens,env);
        std::cout << "assign generate" << std::endl;
    }
    else if(subtokens[0].type == IDENTIFIER && subtokens[1].value == "(")
    {
        if(functions.find(subtokens[0].value) == functions.end()){
            std::cout << "Function " << subtokens[0].value << " not found" << std::endl;
        }else{
            functions[subtokens[0].value]->call(subtokens,env);
            std::cout << "Function " << subtokens[0].value << " generate" << std::endl;
        }
    }
    else if (subtokens[0].type == KEYWORD && subtokens[0].value == "var")
    {
        new Var(subtokens,env);
        std::cout << "var generate" << std::endl;
    }
    else if (subtokens[0].type == KEYWORD && subtokens[0].value == "print")
    {
        new Print(subtokens,env);
        std::cout << "print generate" << std::endl;
    }
    else if(subtokens[0].type==KEYWORD && subtokens[0].value=="if")
    {
        new If(subtokens,env);
    }
    else if(subtokens[0].type==KEYWORD && subtokens[0].value=="while")
    {
        new While(subtokens,env);
    }
    else if(subtokens[0].type==KEYWORD && subtokens[0].value=="break")
    {
        new Break(env);
    }
    else if(subtokens[0].type==KEYWORD&&subtokens[0].value=="continue")
    {
        new Continue(env);
    }
    else if(subtokens[0].type==KEYWORD&&subtokens[0].value=="return")
    {
        if (function_ret_label == "")
        {
            std::cout << "error:unexpected return" << std::endl;
            exit(1);
        }
        else
        {
            if (subtokens.size() == 1)
            {
                new Return(env);
            }
            else
            {
                new Return(subtokens, env);
            }
        }
    }
    else
    {
        std::cout << "error:unexpected token" << std::endl;
    }
}
