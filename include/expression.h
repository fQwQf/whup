#ifndef EXPRESSION_H_
#define EXPRESSION_H_
#include <bits/stdc++.h>

#ifndef PARSER_H_
#include"whup_parser.h"
#endif


std::string newTempVar(std::string type);


class Expr {
private:
    std::vector<Token> E_expr;
    ThreeAddressCode tac; // 三地址码输出

    //创建左右子节点
    Expr* left;
    Expr* right;

    Environment* env;

    void matchPar(int& i);//用来跳过括号内容
    void matchBracket(int& i);//用来检查中括号
    bool is_single_arr(int demension);//用来检查是否只剩下单个数组
public:
    //构造函数用以初始化expression类，传入参数为token集合和env
    Expr(const std::vector<Token>& expr,Environment* env);

    // 合并对expr的所有处理,将得到的三地址码栈压入总栈
    void expr(); 

    //返回计算结果的函数
    std::string getTacResult()
    {
        return tac.result;
    }

    //设置函数让expr指向下一个env
    void setEnv(Environment* env);

    std::string return_type();
};
#endif
