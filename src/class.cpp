#include"class.h"
//全局的类表
std::unordered_map<std::string,std::vector<Token>> class_table;

Class::Class(std::vector<Token>code)
{
    int index=1;//用index访问，略过class
    std::string className=code[index].value;
    index++;index++;//略过classname和{
    class_table[className]=std::vector<Token>(code.begin()+index,code.end());
}