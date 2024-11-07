//ast是whup程序的树形结构。它由节点组成，每个节点代表一个表达式，语句或声明。

#include <iostream>
#include <string>
#include <cctype>
#include <unordered_map>
#include <variant>
#include <vector>

// 令牌类型
enum TokenType {
    TOKEN_NUMBER,  // 表示一个数字
    TOKEN_IDENTIFIER, // 表示标识符
    TOKEN_PLUS,    // 表示 '+' 符号
    TOKEN_MINUS,   // 表示 '-' 符号
    TOKEN_MUL,     // 表示 '*' 符号
    TOKEN_DIV,     // 表示 '/' 符号
    TOKEN_ASSIGN,  // 表示 '=' 符号
    TOKEN_LPAREN,  // 表示 '(' 符号
    TOKEN_RPAREN,  // 表示 ')' 符号
    TOKEN_WHILE,   // 表示 'while' 关键字
    TOKEN_FOR,     // 表示 'for' 关键字
    TOKEN_IF,      // 表示 'if' 关键字
    TOKEN_ELSE,    // 表示 'else' 关键字
    TOKEN_FUNC,    // 表示 'func' 关键字
    TOKEN_RETURN,  // 表示 'return' 关键字
    TOKEN_CLASS,   // 表示 'class' 关键字
    TOKEN_LBRACE,  // 表示 '{' 符号
    TOKEN_RBRACE,  // 表示 '}' 符号
    TOKEN_SEMICOLON, // 表示 ';' 符号
    TOKEN_LT,      // 表示 '<' 符号
    TOKEN_GT,      // 表示 '>' 符号
    TOKEN_EQ,      // 表示 '==' 符号
    TOKEN_NEQ,     // 表示 '!=' 符号
    TOKEN_AND,     // 表示 '&&' 符号
    TOKEN_OR,      // 表示 '||' 符号
    TOKEN_NOT,     // 表示 '!' 符号
    TOKEN_EOF      // 表示输入的结束
};

// 令牌结构，包含类型和值
struct Token {
    TokenType type;
    std::variant<int, std::string> value;
};

// 函数结构，包含函数体和参数
struct Function {
    std::vector<std::string> parameters;
    std::string body;
};

// 类结构，包含类的属性和方法
struct Class {
    std::unordered_map<std::string, double> attributes;
    std::unordered_map<std::string, Function> methods;
};

// 词法分析器类，用于将输入转换为令牌
class Lexer {
public:
    // 构造函数，使用输入文本初始化词法分析器
    Lexer(const std::string& text) : text(text), pos(0) { current_char = text[pos]; }

    // 从输入中获取下一个令牌
    Token get_next_token() {
        while (current_char != '\0') {
            // 跳过空白字符
            if (isspace(current_char)) {
                skip_whitespace();
                continue;
            }

            // 如果当前字符是数字，则返回一个数字令牌
            if (isdigit(current_char)) {
                return Token{ TOKEN_NUMBER, integer() };
            }

            // 如果当前字符是字母，则处理标识符或关键字
            if (isalpha(current_char)) {
                std::string id = identifier();
                if (id == "while") {
                    return Token{ TOKEN_WHILE, id };
                } else if (id == "for") {
                    return Token{ TOKEN_FOR, id };
                } else if (id == "if") {
                    return Token{ TOKEN_IF, id };
                } else if (id == "else") {
                    return Token{ TOKEN_ELSE, id };
                } else if (id == "func") {
                    return Token{ TOKEN_FUNC, id };
                } else if (id == "return") {
                    return Token{ TOKEN_RETURN, id };
                } else if (id == "class") {
                    return Token{ TOKEN_CLASS, id };
                } else {
                    return Token{ TOKEN_IDENTIFIER, id };
                }
            }

            // 如果当前字符是 '+'，则返回一个加号令牌
            if (current_char == '+') {
                advance();
                return Token{ TOKEN_PLUS, 0 };
            }

            // 如果当前字符是 '-'，则返回一个减号令牌
            if (current_char == '-') {
                advance();
                return Token{ TOKEN_MINUS, 0 };
            }

            // 如果当前字符是 '*'，则返回一个乘号令牌
            if (current_char == '*') {
                advance();
                return Token{ TOKEN_MUL, 0 };
            }

            // 如果当前字符是 '/'，则返回一个除号令牌
            if (current_char == '/') {
                advance();
                return Token{ TOKEN_DIV, 0 };
            }

            // 如果当前字符是 '='，则返回一个赋值或相等令牌
            if (current_char == '=') {
                advance();
                if (current_char == '=') {
                    advance();
                    return Token{ TOKEN_EQ, 0 };
                }
                return Token{ TOKEN_ASSIGN, 0 };
            }

            // 如果当前字符是 '<'，则返回一个小于令牌
            if (current_char == '<') {
                advance();
                return Token{ TOKEN_LT, 0 };
            }

            // 如果当前字符是 '>'，则返回一个大于令牌
            if (current_char == '>') {
                advance();
                return Token{ TOKEN_GT, 0 };
            }

            // 如果当前字符是 '!'，则检查是否为不等于或逻辑非
            if (current_char == '!') {
                advance();
                if (current_char == '=') {
                    advance();
                    return Token{ TOKEN_NEQ, 0 };
                }
                return Token{ TOKEN_NOT, 0 };
            }

            // 如果当前字符是 '&'，则检查是否为逻辑与
            if (current_char == '&') {
                advance();
                if (current_char == '&') {
                    advance();
                    return Token{ TOKEN_AND, 0 };
                }
            }

            // 如果当前字符是 '|'，则检查是否为逻辑或
            if (current_char == '|') {
                advance();
                if (current_char == '|') {
                    advance();
                    return Token{ TOKEN_OR, 0 };
                }
            }

            // 如果当前字符是 '('，则返回一个左括号令牌
            if (current_char == '(') {
                advance();
                return Token{ TOKEN_LPAREN, 0 };
            }

            // 如果当前字符是 ')'，则返回一个右括号令牌
            if (current_char == ')') {
                advance();
                return Token{ TOKEN_RPAREN, 0 };
            }

            // 如果当前字符是 '{'，则返回一个左大括号令牌
            if (current_char == '{') {
                advance();
                return Token{ TOKEN_LBRACE, 0 };
            }

            // 如果当前字符是 '}'，则返回一个右大括号令牌
            if (current_char == '}') {
                advance();
                return Token{ TOKEN_RBRACE, 0 };
            }

            // 如果当前字符是 ';'，则返回一个分号令牌
            if (current_char == ';') {
                advance();
                return Token{ TOKEN_SEMICOLON, 0 };
            }
        }
        // 当到达输入末尾时返回 EOF 令牌
        return Token{ TOKEN_EOF, 0 };
    }

private:
    std::string text;  // 输入文本，需要被分解为令牌
    size_t pos;        // 当前在输入文本中的位置
    char current_char; // 当前正在处理的字符

    // 前进到下一个字符
    void advance() {
        pos++;
        if (pos > text.size() - 1)
            current_char = '\0'; // 如果到达输入末尾，则设置为空字符
        else
            current_char = text[pos];
    }

    // 跳过空白字符
    void skip_whitespace() {
        while (current_char != '\0' && isspace(current_char)) {
            advance();
        }
    }

    // 读取连续的数字并返回整数值
    int integer() {
        int result = 0;
        while (current_char != '\0' && isdigit(current_char)) {
            result = result * 10 + (current_char - '0');
            advance();
        }
        return result;
    }

    // 读取标识符
    std::string identifier() {
        std::string result;
        while (current_char != '\0' && isalnum(current_char)) {
            result += current_char;
            advance();
        }
        return result;
    }
};

// 解释器类，用于计算表达式的值
class Interpreter {
public:
    // 构造函数，使用词法分析器初始化解释器
    Interpreter(Lexer& lexer) : lexer(lexer) { current_token = lexer.get_next_token(); }

    // 计算表达式的值
    double expr() {
        // 解析第一个因子
        double result = term();

        // 循环处理加法和减法运算
        while (current_token.type == TOKEN_PLUS || current_token.type == TOKEN_MINUS) {
            Token op = current_token;
            if (op.type == TOKEN_PLUS) {
                eat(TOKEN_PLUS);
                result += term();
            } else if (op.type == TOKEN_MINUS) {
                eat(TOKEN_MINUS);
                result -= term();
            }
        }

        return result;
    }

    // 设置标识符的值
    void assign() {
        if (current_token.type == TOKEN_IDENTIFIER) {
            std::string var_name = std::get<std::string>(current_token.value);
            eat(TOKEN_IDENTIFIER);
            eat(TOKEN_ASSIGN);
            double value = expr();
            variables[var_name] = value;
        } else {
            throw std::runtime_error("赋值语法错误");
        }
    }

    // 处理用户自定义函数定义
    void define_function() {
        eat(TOKEN_FUNC);
        std::string func_name = std::get<std::string>(current_token.value);
        eat(TOKEN_IDENTIFIER);
        eat(TOKEN_LPAREN);

        std::vector<std::string> parameters;
        if (current_token.type == TOKEN_IDENTIFIER) {
            parameters.push_back(std::get<std::string>(current_token.value));
            eat(TOKEN_IDENTIFIER);
            while (current_token.type == TOKEN_COMMA) {
                eat(TOKEN_COMMA);
                parameters.push_back(std::get<std::string>(current_token.value));
                eat(TOKEN_IDENTIFIER);
            }
        }
        eat(TOKEN_RPAREN);
        eat(TOKEN_LBRACE);

        std::string body;
        while (current_token.type != TOKEN_RBRACE && current_token.type != TOKEN_EOF) {
            body += current_char;
            advance();
        }
        eat(TOKEN_RBRACE);

        functions[func_name] = {parameters, body};
    }

    // 处理用户自定义类定义
    void define_class() {
        eat(TOKEN_CLASS);
        std::string class_name = std::get<std::string>(current_token.value);
        eat(TOKEN_IDENTIFIER);
        eat(TOKEN_LBRACE);

        Class new_class;
        while (current_token.type != TOKEN_RBRACE && current_token.type != TOKEN_EOF) {
            if (current_token.type == TOKEN_IDENTIFIER) {
                std::string identifier = std::get<std::string>(current_token.value);
                eat(TOKEN_IDENTIFIER);
                if (current_token.type == TOKEN_ASSIGN) {
                    // 定义属性
                    eat(TOKEN_ASSIGN);
                    double value = expr();
                    new_class.attributes[identifier] = value;
                    eat(TOKEN_SEMICOLON);
                } else if (current_token.type == TOKEN_FUNC) {
                    // 定义方法
                    define_function();
                    new_class.methods[identifier] = functions[identifier];
                }
            }
        }
        eat(TOKEN_RBRACE);

        classes[class_name] = new_class;
    }

    // 调用用户自定义函数
    double call_function(const std::string& func_name) {
        if (functions.find(func_name) == functions.end()) {
            throw std::runtime_error("未定义的函数: " + func_name);
        }

        Function func = functions[func_name];
        std::unordered_map<std::string, double> local_variables;

        eat(TOKEN_LPAREN);
        for (size_t i = 0; i < func.parameters.size(); ++i) {
            if (i > 0) {
                eat(TOKEN_COMMA);
            }
            double value = expr();
            local_variables[func.parameters[i]] = value;
        }
        eat(TOKEN_RPAREN);

        Lexer func_lexer(func.body);
        Interpreter func_interpreter(func_lexer);
        func_interpreter.variables = local_variables;
        func_interpreter.functions = functions;
        func_interpreter.interpret();

        if (func_interpreter.return_value.has_value()) {
            return func_interpreter.return_value.value();
        }
        throw std::runtime_error("函数没有返回值: " + func_name);
    }

    // 处理 return 语句
    void return_statement() {
        eat(TOKEN_RETURN);
        return_value = expr();
    }

    // 解释整个输入
    void interpret() {
        while (current_token.type != TOKEN_EOF) {
            if (current_token.type == TOKEN_IDENTIFIER) {
                std::string identifier = std::get<std::string>(current_token.value);
                eat(TOKEN_IDENTIFIER);
                if (current_token.type == TOKEN_LPAREN) {
                    double result = call_function(identifier);
                    std::cout << "函数返回值: " << result << std::endl;
                } else {
                    assign();
                }
            } else if (current_token.type == TOKEN_FUNC) {
                define_function();
            } else if (current_token.type == TOKEN_CLASS) {
                define_class();
            } else if (current_token.type == TOKEN_RETURN) {
                return_statement();
            } else {
                double result = expr();
                std::cout << "结果: " << result << std::endl;
            }
            current_token = lexer.get_next_token();
        }
    }

private:
    Lexer& lexer;        // 词法分析器的引用
    Token current_token; // 当前正在处理的令牌
    std::unordered_map<std::string, double> variables; // 变量表，用于存储标识符的值
    std::unordered_map<std::string, Function> functions; // 函数表，用于存储用户自定义函数
    std::unordered_map<std::string, Class> classes; // 类表，用于存储用户自定义类
    std::optional<double> return_value; // 用于存储函数返回值

    // 处理乘法和除法的项
    double term() {
        double result = factor();

        // 循环处理乘法和除法运算
        while (current_token.type == TOKEN_MUL || current_token.type == TOKEN_DIV) {
            Token op = current_token;
            if (op.type == TOKEN_MUL) {
                eat(TOKEN_MUL);
                result *= factor();
            } else if (op.type == TOKEN_DIV) {
                eat(TOKEN_DIV);
                double divisor = factor();
                if (divisor == 0) {
                    throw std::runtime_error("除数不能为零");
                }
                result /= divisor;
            }
        }

        return result;
    }

    // 解析一个因子（数字、标识符或函数调用）
    double factor() {
        Token token = current_token;
        if (token.type == TOKEN_NUMBER) {
            eat(TOKEN_NUMBER);
            return std::get<int>(token.value);
        } else if (token.type == TOKEN_IDENTIFIER) {
            std::string var_name = std::get<std::string>(token.value);
            eat(TOKEN_IDENTIFIER);
            if (current_token.type == TOKEN_LPAREN) {
                return call_function(var_name);
            } else if (variables.find(var_name) != variables.end()) {
                return variables[var_name];
            } else {
                throw std::runtime_error("未定义的变量: " + var_name);
            }
        } else {
            throw std::runtime_error("解析因子时出错");
        }
    }

    // 如果当前令牌类型与预期类型匹配，则消费该令牌
    void eat(TokenType token_type) {
        if (current_token.type == token_type) {
            current_token = lexer.get_next_token();
        } else {
            throw std::runtime_error("解析输入时出错");
        }
    }
};

int main() {
    std::string text;
    std::cout << "输入一个表达式、赋值语句、函数定义或类定义（例如：class MyClass { x = 10; func add(a, b) { return a + b; } } 或 MyClass.add(3, 4)）：";
    std::getline(std::cin, text);

    Lexer lexer(text);              // 使用输入文本创建词法分析器
    Interpreter interpreter(lexer); // 使用词法分析器创建解释器
    try {
        interpreter.interpret(); // 解释整个输入
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl; // 处理任何解析错误
    }

    return 0;
} 


