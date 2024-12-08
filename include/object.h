#ifndef BLOCK_H_
#include"block.h"
#endif

#ifndef PARSER_H_
#include"whup_parser.h"
#endif

#ifndef CLASSFUNCTION_H_
#include"classfunction.h"
#endif

#ifndef OBJECT_H_
#define OBJECT_H_
//创建对象实例
class Object
{
    Environment* Object_env;//此env用全局环境来初始化，即在实例声明时，将全局环境传入。
    std::string Object_name;
    std::string Class_name;
    
    
    public:
    //这里的指针式object所处的环境，用来回溯到全局环境
    Object(std::string className,std::string objectName,Environment* env);
    void var_declare(std::vector<Token> tokens);//变量声明
    void function_declare(std::vector<Token> tokens);//函数声明
    void constuctor_declare(std::vector<Token>);//构造函数声明


    void generator(std::vector<Token>subtoken);//整合变量声明与函数声明//其实没有必要但好看
    void matchBrace(int &i,std::vector<Token> &tokens);
    void matchPar(int &i,std::vector<Token> &tokens);
    void copy(Object*ptrObject);//复制对象
    
    
    ClassFunction*myConstructor;//单独的构造函数
    std::unordered_map<std::string,ClassFunction*> function_table;//函数表//<函数名，函数指针>
};



#endif