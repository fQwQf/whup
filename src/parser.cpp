#include <bits/stdc++.h>
#include "whup_parser.h"


std::vector<ThreeAddressCode> tacs;  // 存储三地址代码的向量
std::unordered_map<std::string, std::string> var_declares;  // 存储将放入c++中变量名和类型的哈希表

int tempVarCounter = 0;  // 临时变量计数器
int tempLabelCounter = 0;  // 临时标签计数器
int global_env_id = 0;  //全局EnvironmentID计数器

std::string newTempVar(std::string type) {
    std::string t = "t" + std::to_string(++tempVarCounter);
    var_declares[t] = type;
    return t;
}

std::string newTempLabel() {
    return "l" + std::to_string(++tempLabelCounter);
}


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
*- get_var：查找一个变量名，返回类型。  
*- change_type_var：修改一个变量的类型。   
*/

/*
构造函数，用于初始化一个新的environment对象，并将其父符号表设置为传入的指针p。
传入一个指向父环境的指针，或者不传入(不传入一般表示根节点)。
*/
Environment::Environment(Environment *p) : parent(p)
{
    id = global_env_id;
    global_env_id++;
    
}
Environment::Environment() : parent(nullptr) {
    id = global_env_id;
    global_env_id++;
}

/**
 * 简介： 向变量表中插入一个新变量。
 *
 * 该函数接受一个字符串参数作为变量名，并将其添加到变量表中，
 * 对应的值初始化为字符串"null"。
 *
 * 参数：
 *  name 要插入的变量名。
 */
void Environment::insert_var(std::string name)
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
std::string Environment::get_var(std::string name)
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
            return parent->get_var(name);
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
void Environment::change_type_var(std::string name, std::string t)
{
    if (var_table.find(name) != var_table.end())
    {
        var_table[name] = t;
    }
    else
    {
        if (parent == nullptr)
        {
            return;
        }
        else
        {
            parent->change_type_var(name, t);
        }
    }
    var_declares[get_var(name)] = t;//并非在符号表创建条目时就插入var_declares，这样只声明未赋值的变量就不会被编译
    return;
}

/*
该函数用于在变量表中查找指定名称的变量类型。
如果当前作用域中不存在该变量，则会递归地在父作用域中查找。

参数：
name 要查找的变量的名称
*/
std::string Environment::get_type_var(std::string name){
    if (var_table.find(name) != var_table.end())
    {
        return var_table[name];
    }
    else
    {
        if (parent == nullptr)
        {
            return "notfound";
        }
        else
        {
            return parent->get_var(name);
        }
    }
    return "notfound";
}
