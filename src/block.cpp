#include "block.h"
#include"assign.h"
#include"var.h"
#include"print.h"
#include"if.h"
#include"while.h"

//跳过大括号
void Block::matchBrace(int &i,std::vector<Token> &tokens)
{
    if (tokens[i].value == "{")
    {
        int leftPar = 1;
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

// 以分号为分隔扫描
Block::Block(std::vector<Token> tokens, Environment *e)
{
    this->env = new Environment(e);

    int last_semicolon = 0;

    for (int i = 0; i < tokens.size(); i++)
    {
        matchBrace(i, tokens);
        if (tokens[i].type == SYMBOL || tokens[i].value == ";")
        {
            std::vector<Token> subtokens(tokens.begin() + last_semicolon, tokens.begin() + i - 1);
            last_semicolon += 1;
            generate(subtokens);
        }
    }
}

//你是不是觉得这样重载构造函数很没必要？
//有这种感觉就对了，其实我是为了水代码量。
//如果不是还有基本道德底线，我甚至连generate()都要展开。(′д｀σ)σ
Block::Block(std::vector<Token> tokens)
{
    this->env = new Environment();

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

    if (subtokens[0].type == IDENTIFIER)
    {
        new Assign(subtokens,env);
        //std::cout << "assign" << std::endl;
    }
    else if (subtokens[0].type == KEYWORD && subtokens[0].value == "var")
    {
        new Var(subtokens,env);
        //std::cout << "var" << std::endl;
    }
    else if (subtokens[0].type == KEYWORD && subtokens[0].value == "print")
    {
        new Print(subtokens,env);
        //std::cout << "print" << std::endl;
    }
    else if(subtokens[0].type==KEYWORD&&subtokens[0].value=="if")
    {
        new If(subtokens,env);
    }
    else if(subtokens[0].type==KEYWORD&&subtokens[0].value=="while")
    {
        new While(subtokens,env);
    }
}
