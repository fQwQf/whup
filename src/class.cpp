#include"class.h"
//全局的类表
std::unordered_map<std::string,Class*> class_table;//类表//<类名，类指针>
Class::Class(std::vector<Token> code)
{
    int index = 1; // 略过 'class'
    std::string className = code[index].value;
    index++; index++; // 略过类名和 '{'
    while (code[index].value != "}")
    {
        statements.push_back(code[index]);
        index++;
    }
    // 检查类是否已存在
    if (class_table.find(className) == class_table.end())
    {
        class_table[className] = this;
        std::cout<<"add class "<<className<<std::endl;
        std::cout<<"class body is "<<std::endl;
        for(auto &i:class_table[className]->statements)
        {
            std::cout<<i.value<<" ";
        }
    }
    else
    {
        std::cout << "警告：类 " << className << " 已存在，未重复添加。" << std::endl;
    }
}
