#include"class.h"
//全局的类表
std::unordered_map<std::string,Class*> class_table;
Class::Class(std::vector<Token> code)
{
    int index = 1; // 略过 'class'
    std::string className = code[index].value;
    index++;index++;  // 略过类名

    statements=std::vector<Token>(code.begin()+index,code.end()-1);

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
