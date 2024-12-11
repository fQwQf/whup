// mingw-w64里有一个parser.h，为避免冲突改名
#ifndef PARSER_H_
#define PARSER_H_

#include <bits/stdc++.h>

#ifndef LEXER_H_
#include "lexer.h"
#endif

#ifndef FUNCTION_H_
#include "function.h"
#endif

class Function;//前向声明

#ifndef FUNCTION_H_
#include "function.h"
#endif

class Function;//前向声明

// 创建新的临时变量
std::string newTempVar(std::string type);
// 创建新的临时标签
std::string newTempLabel();

/*
 *结构：三地址码
 *result = arg1 op arg2 ;
 *储存四个字符串，分别代表运算符，操作数1，操作数2，结果
 */
enum Operator
{
    ADD, // +
    STRADD,//字符串相加
    SUB, // -
    MUL, // *
    DIV, // /
    MOD, // %
    AND, // &&
    OR,  // ||
    NOT, // !
    EQ,  // ==
    NEQ, // !=
    LT,  // <
    GT,  // >
    LE,  // <=
    GE,  // >=
    ASSIGN, // =
    STRASSIGN,//字符串赋值
    LABEL,  // label
    GOTO,   // goto
    IF_GOTO, // if_goto
    PRINT,   // print
    WINPUT,   // input
    PUSH,    // push
    POP,     // pop
    CALL,    // call
    RET,     // return
    EXIT,    // end
    POW,    // **
    REFSTR,
    REFNUM,      //引用传递
    ARRSET,  //数组声明
    ARRASSIGN,//数组赋值
    ARRGET,  //数组取值
    ARRLEN,  //数组长度
    BIASNUM,     //数字数组偏移
    BIASSTR      //字符串
};

struct ThreeAddressCode
{
    Operator opperator;       // 操作符
    std::string op;     // 操作符//这里是为了方便，因为在生成代码的时候需要用到字符串
    std::string arg1;   // 变量1
    std::string arg2;   // 变量2
    std::string result; // 存储结果的变量

    ThreeAddressCode(Operator o,std::string s1,std::string s2,std::string res):opperator(o),arg1(s1),arg2(s2),result(res){}
    ThreeAddressCode(){}
    ThreeAddressCode(Operator o,std::string op,std::string s1,std::string s2,std::string res):opperator(o),op(op),arg1(s1),arg2(s2),result(res){}
};

/*
 *创建环境类
 *
 *定义一个符号表类，具有链表结构，有一个指向父符号表的指针。
 *叫environment是为了致敬龙书。
 *内部有一个private的unordered_map，用来存储变量名，类型。
 *每个新增的对象都有一个独特的id。
 *除此之外应该还有函数表和类表，不过之后再说
 *
 *具有以下几个方法：
 *
 *- insert_var：插入一个变量名，默认类型null。
 *- get_var：查找一个变量名，返回类型。
 *- change_type_var：修改一个变量的类型。
 */
class Environment
{
private:
    Environment *parent; // 指向父环境的指针
    int id;
    static int i;
    

public:
    // 以哈希表创建符号表，用于存储变量名及其类型
    std::unordered_map<std::string, std::string> var_table;
    std::unordered_map<std::string, std::string> arr_table;

    // 储存存储返回值的临时变量，便于压入栈帧
    // 放在environment是因为栈帧的另一个组成部分，即局部变量，也在environment，这样统一性更好
    std::vector<std::string> return_var_list;

    // 存储函数名和对应的对象指针哈希表
    std::unordered_map<std::string, Function*> function_table;  

    bool is_import = false; // 是否为导入环境


    /*
    构造函数，用于初始化一个新的environment对象，并将其父符号表设置为传入的指针p。
    传入一个指向父环境的指针，或者不传入(不传入一般表示根节点)。
    */
    Environment(Environment *p);
    Environment();

    /**
     * 简介： 向变量表中插入一个新变量。
     *
     * 该函数接受一个字符串参数作为变量名，并将其添加到变量表中，
     * 对应的值初始化为字符串"null"。
     *
     * 参数：
     *  name 要插入的变量名。
     */
    void insert_var(std::string name);
    void insert_arr(std::string name);
    /**
     * 简介： 查找变量的函数
     *
     * 该函数用于在变量表中查找指定名称的变量。如果当前作用域中存在该变量，则返回其值；
     * 如果不存在，则会递归地在父作用域中查找。如果最终未找到变量，则返回"null"。
     *
     * 参数：
     *  name 要查找的变量在whup中的的名称
     * 返回值： 变量翻译成的c++的名称，如果未找到则返回"null"
     */
    std::string get_var(std::string name);
    std::string get_arr(std::string name);

    /**
     * 简介： 更改变量类型的函数
     *
     * 该函数用于在变量表中查找指定名称的变量，并将其类型更改为提供的新类型。
     * 如果当前作用域中不存在该变量，则会递归地在父作用域中查找并尝试更改类型。
     *
     * 参数：
     *  name 要更改类型的变量的名称
     *  t 新的变量类型
     */
    void change_type_var(std::string name, std::string t);
    void change_type_arr(std::string name, std::string t);

    /**
     * 函数名：get_type_var
     *
     * 该函数用于在变量表中查找指定名称的变量类型。
     * 如果当前作用域中不存在该变量，则会递归地在父作用域中查找。
     *
     * 参数：
     *  name 要查找的变量的名称
     */
    std::string get_type_var(std::string name);

    // 为了使类的实例在某个Block内声明时一定查找到全局变量，设计一个得到全局Env的函数
    Environment *backToGlobal();

    // 插入一个存储返回值的临时变量
    void insert_return_var(std::string name);

    //检测是否为全局环境
    bool isGlobal();

    // 查找一个函数，方法和查变量一样
    Function* get_function(std::string name);

};

#endif