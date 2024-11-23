#ifndef LEXER_H_
#define LEXER_H_
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <unordered_set>
#include <stack>
#include <memory>

enum TokenType {
    KEYWORD, IDENTIFIER, NUMBER, STRING, SYMBOL, EOF_TOKEN
};

//词法单元，包含类型、值和所在行数
struct Token {
    TokenType type;
    std::string value;
    int line_number;//记录行号，便于定位错误
    bool processed = false;//标记是否已经经过处理

    Token(TokenType type, const std::string& value, int line_number);
    Token();
};


//定义Lexer（词法分析器类），用于对输入的字符串进行解析，将其分解为一系列 Token，以供语法分析器或编译器使用。
class Lexer {

public:
    Lexer(const std::string &input);

    //核心功能函数，将input转化为一系列token的集合
    std::vector<Token> tokenize();

private:
    std::string input;
    size_t pos;//当前解析位置的索引
    int line;//当前解析位置的行号

    //查看当前位置之后的字符
    char peek(int offset);
    //获取当前字符，并移动指针到下一位置
    char next();
    //跳过空白字符
    void skip_whitespace();
    //跳过注释内容
    void skip_comment();
    //读取关键字和标识符
    Token read_keyword_or_identifier();
    //读取数字
    Token read_number();
    //读取双引号字符串
    Token read_string_1();
    //读取单引号字符
    Token read_string_2();
    //读取操作符
    Token read_symbol();
    //判断字符串是否为关键字
    bool is_keyword(const std::string& word);
    //判断字符串是否为运算符
    bool is_symbol_set(const std::string& word);


    std::unordered_set<std::string> keywords() const;//关键字的集合
    std::unordered_set<std::string> symbol_sets() const;//运算符的集合


};
#endif