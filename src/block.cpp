#include "block.h"
#include <bits/stdc++.h>

// 以分号为分隔扫描
Block::Block(std::vector<Token> tokens, Environment *env)
{
    this->env = new Environment(env);

    int last_semicolon = 0;

    for (int i = 0; i < tokens.size(); i++)
    {
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
        if (tokens[i].type == SYMBOL || tokens[i].value == ";")
        {
            std::vector<Token> subtokens(tokens.begin() + last_semicolon, tokens.begin() + i - 1);
            last_semicolon += 1;
            generate(subtokens);
        }
    }
}

// 根据首token传入对应的类的构造函数中。
void Block::generate(std::vector<Token> subtokens)
{
    if (subtokens[0].type == IDENTIFIER)
    {
        new Assign(subtokens);
    }
    else if (subtokens[0].type == KEYWORD && subtokens[0].value == "var")
    {
        // new Var(subtokens);
    }
}
