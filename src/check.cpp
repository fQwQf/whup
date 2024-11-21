#include "check.h"

bool check::checkPar(std::vector<Token>code)
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
                std::cerr << "Error: Unmatched ')' at line " << c.line_number << std::endl;
                return false;
            }
        brackets.pop();
        }
    }
    if (!brackets.empty()) 
    {
        std::cerr << "Error: Unmatched '(' at line " << line_number << std::endl;
        return false;
    }
    return true;
}

bool check::checkBracket(std::vector<Token>code)
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
                std::cerr << "Error: Unmatched ']' at line " << c.line_number << std::endl;
                return false;
            }
        brackets.pop();
        }
    }
    if (!brackets.empty()) 
    {
        std::cerr << "Error: Unmatched '[' at line " << line_number << std::endl;
        return false;
    }
    return true;
}

bool check::checkBrace(std::vector<Token>code)
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
                std::cerr << "Error: Unmatched '}' at line " << c.line_number << std::endl;
                return false;
            }
        brackets.pop();
        }
    }
    if (!brackets.empty()) 
    {
        std::cerr << "Error: Unmatched '{' at line " << line_number << std::endl;
        return false;
    }
    
    return true;
}
