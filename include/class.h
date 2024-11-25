#ifndef CLASSTABLE_H_
#define CLASSTABLE_H_
#include <bits/stdc++.h>
#include"whupc.h"


class Class
{
    std::vector<Token> statements;//用于储存定义语句
    public:
    //方法集合，实现将一个类插入类表
    Class(std::vector<Token>code);

    std::vector<Token>& getStatements()
    {
        return statements;
    }
};
#endif