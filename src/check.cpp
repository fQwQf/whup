#include "check.h"

static std::vector<Error> errors; // 存储错误信息

void checkSyntax::checkID(Token token)
{
    for(auto ch:token.value)
    {
        //如果标识符不是由数字、字母或 _ 组成，则输出错误信息
        if (!(isalpha(ch) || isdigit(ch) || ch == '_')) 
        {
            errors.push_back({ token.line_number, "Error: Unrecognized token '" + token.value + "' at line " });
        }
    }
}

void checkSyntax::checkVar(std::string name,Environment *env,int line_number)
{
    if ("null"==env->get_var(name)) 
    {
        errors.push_back({ line_number, "Error: Unrecognized token '" + name + "' at line " });
    }
}

void checkBrackets::checkPar(std::vector<Token>code)
{
    int line_number=0;
    std::stack<char> brackets;
    for (auto c : code) 
    {
        if (c.value == "(") 
        {
            brackets.push('(');
            line_number=c.line_number;
        } 
        else if (c.value == ")") 
        {
            if (brackets.empty()) 
            {
                
                errors.push_back({ c.line_number, "Error: Unmatched ')' at line " });
            }
        brackets.pop();
        }
    }
    if (!brackets.empty()) 
    {
        errors.push_back({ line_number, "Error: Unmatched '(' at line " });
    }

    return ;
}

void checkBrackets::checkBracket(std::vector<Token>code)
{
    int line_number=0;
    std::stack<char> brackets;
    for (auto c : code) 
    {
        if (c.value == "[") 
        {
            brackets.push('[');
            line_number=c.line_number;
        } 
        else if (c.value == "]") 
        {
            if (brackets.empty()) 
            {
                errors.push_back({ c.line_number, "Error: Unmatched ']' at line " });
            }
        brackets.pop();
        }
    }
    if (!brackets.empty()) 
    {
        errors.push_back({ line_number, "Error: Unmatched '[' at line " });
    }

    return ;
}

void checkBrackets::checkBrace(std::vector<Token>code)
{
    int line_number=0;
    std::stack<char> brackets;
    for (auto c : code) 
    {
        if (c.value == "{") 
        {
            brackets.push('{');
            line_number=c.line_number;
        } 
        else if (c.value == "}") 
        {
            if (brackets.empty()) 
            {
                errors.push_back({ c.line_number, "Error: Unmatched '}' at line " });
            }
        brackets.pop();
        }
    }
    if (!brackets.empty()) 
    {
        errors.push_back({ line_number, "Error: Unmatched '{' at line " });
    }

    return ;
}

void CheckSemicolon::checkCode(const std::string& code) 
{
    std::istringstream codeStream(code);//将code按行分块
    std::string line;
    int lineNumber = 0;

    while (std::getline(codeStream, line)) //getline读入codeStream流，默认按换行符分块，并存入line中 
    {
        lineNumber++;
        checkLine(line, lineNumber);
    }
}

void CheckSemicolon::checkLine(const std::string& line, int lineNumber) 
{
    std::string trimmedLine = trim(line);//去掉收尾空白符后的line
    // 忽略空行和以 '{' 或 '}' 开头的块
    if (trimmedLine.empty() || trimmedLine == "{" || trimmedLine == "}") {
        return;
    }
    // 如果行没有以分号结尾且不是块结构，报错
    if (!endsWithSemicolon(trimmedLine) && !isBlockStructure(trimmedLine)) {
        errors.push_back({lineNumber, "Error: Missing semicolon at the end of the line "});
    }
}

bool CheckSemicolon::endsWithSemicolon(const std::string& line) 
{
    return !line.empty() && line.back() == ';';
}

bool CheckSemicolon::isBlockStructure(const std::string& line) 
{
    return line.find("{") != std::string::npos || line.find("}") != std::string::npos;
}

std::string CheckSemicolon::trim(const std::string& str) 
{
    size_t first = str.find_first_not_of(" \t");
    if (first == std::string::npos) 
    {
        return ""; // 全是空白字符
    }
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, last - first + 1);
}

void printErrors() 
{
    if (errors.empty()) 
    {
        /*
        std::cout << "No errors found." << std::endl;
        若没有发现错误，也可直接不输出
        */
    }
    else 
    {
        for (const auto& error : errors) 
        {
            //输出错误信息，其中"\033[31m"表示输出红色，"\033[0m"表示恢复默认颜色
            std::cerr << "\033[31m" << error.message << error.line << "\033[0m" << std::endl;
        }
        exit(1);//结束程序
    }
}