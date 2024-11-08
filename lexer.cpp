/*
词法分析器将输入的字符串中的词素转化为词法单元。词素是单词，符号，数字等。  
变量作用域在词法分析器中处理，具体方法是对不同作用域的变量名在词法分析中进行区分。  
词法分析器输出结果应包含代码所在行，便于调试。  
*/
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <unordered_set>

enum class TokenType {
    KEYWORD, IDENTIFIER, NUMBER, STRING, SYMBOL, EOF_TOKEN
};

struct Token {
    TokenType type;
    std::string value;
    int line_number;

    Token(TokenType t, const std::string& v, int ln) : type(t), value(v), line_number(ln) {}
};

class Lexer {
public:
    Lexer(const std::string& input) : input(input), pos(0), line(1) {}

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

    std::unordered_set<std::string> keywords() const;
};

char Lexer::peek(int offset=0) {
    if (pos >= input.size()) return '\0';
    return input[pos + offset];
}

char Lexer::next() {
    if (pos < input.size()) {
        char c = input[pos++];
        if (c == '\n') line++;
        return c;
    }
    return '\0';
}

void Lexer::skip_whitespace() {
    while (std::isspace(peek())) next();
}

void Lexer::skip_comment() {
    if (peek() == '/' && peek(1) == '/') {
        while (peek() != '\n' && peek() != '\0') next();
    } else if (peek() == '/' && peek(1) == '*') {
        next(); next();
        while (!(peek() == '*' && peek(1) == '/')) {
            if (peek() == '\0') throw std::runtime_error("Unterminated comment");
            next();
        }
        next(); next();
    }
}

Token Lexer::read_keyword_or_identifier() {
    std::string result;
    while (std::isalnum(peek()) || peek() == '_') {
        result += next();
    }
    if (is_keyword(result)) {
        return Token(TokenType::KEYWORD, result, line);
    }
    return Token(TokenType::IDENTIFIER, result, line);
}

Token Lexer::read_number() {
    std::string result;
    while (std::isdigit(peek()) || peek() == '.') {
        result += next();
    }
    return Token(TokenType::NUMBER, result, line);
}

Token Lexer::read_string_1() {
    std::string result;
    next(); // 跳过引号
    while (peek() != '"' && peek() != '\0') {
        result += next();
    }
    if (peek() == '"') next();
    return Token(TokenType::STRING, result, line);
}

Token Lexer::read_string_2() {
    std::string result;
    next(); // 跳过引号
    while (peek() != '\'' && peek() != '\0') {
        result += next();
    }
    if (peek() == '\'') next();
    return Token(TokenType::STRING, result, line);
}

Token Lexer::read_symbol() {
    std::string result(1, next());
    return Token(TokenType::SYMBOL, result, line);
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (true) {
        skip_whitespace();
        skip_comment();
        if (peek() == '\0') {
            tokens.push_back(Token(TokenType::EOF_TOKEN, "", line));
            break;
        } else if (std::isalpha(peek()) || peek() == '_') {
            tokens.push_back(read_keyword_or_identifier());
        } else if (std::isdigit(peek())) {
            tokens.push_back(read_number());
        } else if (peek() == '"') {
            tokens.push_back(read_string_1());
        } else if (peek() == '\'') {
            tokens.push_back(read_string_2());
        } else if (peek() != '\n'){
            tokens.push_back(read_symbol());
        }
    }
    return tokens;
}

bool Lexer::is_keyword(const std::string& word) {
    static const std::unordered_set<std::string> keyword_set = keywords();
    return keyword_set.find(word) != keyword_set.end();
}

std::unordered_set<std::string> Lexer::keywords() const {
    return {
        "int", "char", "float", "double", "bool", "void",
        "if", "else", "for", "while", "return"
    };
}

int main() {
    std::string code = R"(
        // 这是一个注释
        int main() {
            int a = 10; // 变量声明
            float b = 3.14;
            /* 多行
               注释 */
            char c = 'csdf';
            return 0;
        }
    )";

    Lexer lexer(code);
    std::vector<Token> tokens = lexer.tokenize();

    for (const auto& token : tokens) {
        std::cout << "Type: " << static_cast<int>(token.type) << ", Value: " << token.value << ", Line: " << token.line_number << std::endl;
    }

    return 0;
}