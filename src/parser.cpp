#include <bits/stdc++.h>
#include "whup_parser.h"
#include "function.h"
#include "WHUPstream.h"

extern WHUPstream_compile1 WHUPout;

std::vector<ThreeAddressCode> tacs;  // 存储三地址代码的向量
std::vector<ThreeAddressCode> function_tacs; // 存储函数内三地址代码的向量
std::unordered_map<std::string, std::string> var_declares;  // 存储将放入c++中变量名和类型的哈希表
std::unordered_map<std::string, Function*> functions;  // 存储函数名和对应的对象指针哈希表，用于调用函数
std::unordered_map<std::string, Environment*> namespace_table; // 存储命名空间名和对应的Environment对象的哈希表

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
void Environment::insert_arr(std::string name)
{
    arr_table[name] = "null";
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
        // if(var_table[name]=="null")//用来禁止未确定类型的变量被使用
        // {
        //     WHUPout << "Variable "+name+" not define type" << std::endl;
        //     exit(1);
        // }
        return name + '_' + std::to_string(id) + '_' + var_table[name];
    }
    else
    {
        if (parent == nullptr)
        {

            WHUPout << "Variable "+name+" not found,return null" << std::endl;
            if(var_declares.find(name)==var_declares.end())
            return "null";
            else
            return name;
        }
        else
        {
            return parent->get_var(name);
        }
    }
    return "null";

}

std::string Environment::get_arr(std::string name)
{
    if (arr_table.find(name) != arr_table.end())
    {
        // if(arr_table[name]=="null")//用来禁止未确定类型的变量被使用
        // {
        //     WHUPout << "Variable "+name+" not define type" << std::endl;
        //     exit(1);
        // }
        return name + '_' + std::to_string(id) + '_' + arr_table[name];
    }
    else
    {
        if (parent == nullptr)
        {

            WHUPout << "Array "+name+" not found,return null" << std::endl;
            return "null";
        }
        else
        {
            return parent->get_arr(name);
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
        //TODO 添加类型检查，如果不是由null改成其它或没有改变，就报错
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
// void Environment::change_type_arr(std::string name, std::string t)
// {
//     if (arr_table.find(name) != arr_table.end())
//     {
//         arr_table[name] = t;
//     }
//     else
//     {
//         if (parent == nullptr)
//         {
//             return;
//         }
//         else
//         {
//             parent->change_type_arr(name, t);
//         }
//     }
//     var_declares[get_arr(name)] = t;//并非在符号表创建条目时就插入var_declares，这样只声明未赋值的变量就不会被编译
//     return;
// }

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
            if(var_declares.find(name)!=var_declares.end())
            {
                WHUPout<<"find "<<name<<" in var_declares"<<std::endl;
                return var_declares[name];
            }
            // else
            // {
            //     WHUPout << "Variable "+name+" not define type" << std::endl;
            //     exit(1);
            // }
            return "notfound";
        }
        else
        {
            return parent->get_type_var(name);
        }
    }
    return "notfound";
}


Environment* Environment::backToGlobal()
{
    Environment*tempptr=this;
    while(tempptr->parent!=nullptr)
    {
        tempptr=tempptr->parent;
    }
    return tempptr;
}

void Environment::insert_return_var(std::string name){
    return_var_list.push_back(name);
}

//开始打算用这个判断是否需要压栈帧，但后来发现在命名空间内也不用压
bool Environment::isGlobal()
{
    if(parent==nullptr || is_import)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// 查到import的environment就停止，防止调用到namespace外
Function* Environment::get_function(std::string name){
    if(function_table.find(name)!=function_table.end())
    {
        return function_table[name];
    }
    else
    {
        if(parent==nullptr)
        {
            // TODO 报错
            return nullptr;
        }
        else
        {
            return parent->get_function(name);
        }
    }
};