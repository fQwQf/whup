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

struct Token {
    TokenType type;
    std::string value;
    int line_number;

    Token(TokenType type, const std::string& value, int line_number);
};



class Lexer {

public:
    Lexer(const std::string &input);

    std::vector<Token> tokenize();

private:
    std::string input;
    size_t pos;
    int line;

    char peek(int offset);
    char next();
    void skip_whitespace();
    void skip_comment();
    Token read_keyword_or_identifier();
    Token read_number();
    Token read_string_1();
    Token read_string_2();
    Token read_symbol();
    bool is_keyword(const std::string& word);
    bool is_symbol_set(const std::string& word);


    std::unordered_set<std::string> keywords() const;
    std::unordered_set<std::string> symbol_sets() const;


};