#include <bits/stdc++.h>


/*
*定义一个符号表类，具有链表结构，有一个指向父符号表的指针。
*叫environment是为了致敬龙书。  
*内部有一个private的unordered_map，用来存储变量名，类型。  
*每个新增的对象都有一个独特的id。
*除此之外应该还有函数表和类表，不过之后再说
*
*具有以下几个方法：  
*
*- insert_var：插入一个变量名，默认类型null。  
*- find_var：查找一个变量名，返回类型。  
*- change_type_var：修改一个变量的类型。   
*/
class environment{
    private:

    std::unordered_map<std::string,std::string> var_table;
    environment* parent;
    int id;
    static int i;


    public:

    /*构造函数，用于初始化一个新的environment对象，并将其父符号表设置为传入的指针p。
    传入一个指向父环境的指针，或者不传入。*/
    environment(environment* p):parent(p){
        id = environment::i;
        environment::i ++;
    }
    environment() : parent(nullptr) {}

    /**
     * 简介： 向变量表中插入一个新变量。
     *
     * 该函数接受一个字符串参数作为变量名，并将其添加到变量表中，
     * 对应的值初始化为字符串"null"。
     *
     * 参数： 
     *  name 要插入的变量名。
     */
    void insert_var(std::string name)
    {
        var_table[name] = "null";
    }
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
    std::string find_var(std::string name)
    {
        if (var_table.find(name) != var_table.end())
        {
            return name + '_' + std::to_string(id) + '_' + var_table[name];
        }
        else
        {
            if (parent == nullptr)
            {
                return "null";
            }
            else
            {
                return parent->find_var(name);
            }
        }
        return "null";
    }

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
    void change_type_var(std::string name,std::string t){
        if(var_table.find(name)!=var_table.end()){
            var_table[name]=t;
        }
        else{
            if(parent==nullptr){
                return;
            }
            else{
                parent->change_type_var(name,t);
            }
        }
        return;
    }
};

int environment::i = 0;


