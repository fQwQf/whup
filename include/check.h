#include"whupc.h"
#include"lexer.h"

//定义Error类及数组，用于存储所有错误
struct Error {
    Token token;
    //int line;
    std::string message;
};

void pushErrors(Token token, std::string message);

//检查句法是否正确
class checkSyntax
{
public:
    //检查标识符是否合法
    static void checkID(Token token);

    //检查变量是否未声明
    static void checkVar(std::string name,Environment *env,int line_number,Token token,bool isArr=false);

};

//用于检查括号的类
class checkBrackets
{
public:
    //检查小括号是否匹配
    static void checkPar(std::string code,std::string file_name);
    //检查中括号是否匹配
    static void checkBracket(std::string code,std::string file_name);
    //检查大括号是否匹配
    static void checkBrace(std::string code,std::string file_name);

};

class CheckSemicolon 
{
public:
    // 检查代码中的分号
    static void checkCode(const std::string& code,std::string file_name);

    // 检查某一行的分号(主要的检查函数)
    static void checkLine(const std::string& line,const std::string& nextline,int lineNumber,std::string file_name);

private:
    // 检查是否以分号结尾
    static bool endsWithSemicolon(const std::string& line);

    // 判断是否是块结构
    static bool isBlockStructure(const std::string& line) ;

    // 去掉行首和行尾的空白字符
    static std::string trim(const std::string& str) ;

};


//输出所有错误信息
void printErrors();
