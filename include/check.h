#include"whupc.h"
// #include"lexer.h"

//定义Error类及数组，用于存储所有错误
struct Error {
    int line;
    std::string message;
};


//检查句法是否正确
class checkSyntax
{
public:
    //检查标识符是否合法
    static void checkID(Token token);

    //检查变量是否未声明
    static void checkVar(std::vector<Token>code);

};

//用于检查括号的类
class checkBrackets
{
public:
    //检查小括号是否匹配
    static void checkPar(std::vector<Token>code);
    //检查中括号是否匹配
    static void checkBracket(std::vector<Token>code);
    //检查大括号是否匹配
    static void checkBrace(std::vector<Token>code);

};

class CheckSemicolon 
{
public:
    // 检查代码中的分号
    static void checkCode(const std::string& code);

    // 检查某一行的分号(主要的检查函数)
    static void checkLine(const std::string& line, int lineNumber);

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
