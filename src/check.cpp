#include "check.h"
#include "whup_io.h"

static std::vector<Error> errors; // 存储错误信息

void checkSyntax::checkID(Token token)
{
    for(auto ch:token.value)
    {
        //如果标识符不是由数字、字母或 _ 组成，则输出错误信息
        if (!(isalpha(ch) || isdigit(ch) || ch == '_')) 
        {
            errors.push_back({ token, "Unrecognized token '" + token.value + "' at line " });
        }
    }
}

void checkSyntax::checkVar(std::string name,Environment *env,int line_number,Token token)
{
    if ("null"==env->get_var(name)) 
    {
        errors.push_back({ token, "Unrecognized token '" + name + "' at line " });
    }
}

void checkBrackets::checkPar(std::string code,std::string file_name)
{
    std::istringstream codeStream(code);//将code按行分块
    std::string line;//记录每一行的内容
    int lineNumber = 0;
    std::stack<int> brackets;
    while(std::getline(codeStream, line))
    {
        lineNumber++;
        for (auto c : line) 
        {
            if (c == '(') 
            {
                brackets.push(lineNumber);
                // line_number=c.line_number;
            }        
            else if (c == ')') 
            {
                //检测到')'，若栈不为空，则缺少'(',报错
                if (brackets.empty()) 
                {
                    Token token(SYMBOL,"",lineNumber,file_name);
                    errors.push_back({ token, "Unmatched ')' at line " });
                }
            brackets.pop();//'('出栈
            }
        }
    }

    //若全部读取后栈不为空，则缺少')'，报错
    if (!brackets.empty()) 
    {
        Token token(SYMBOL,"",brackets.top(),file_name);
        errors.push_back({ token , "Unmatched '(' at line " });
    }

    return ;
}

void checkBrackets::checkBracket(std::string code,std::string file_name)
{
    std::istringstream codeStream(code);//将code按行分块
    std::string line;//记录每一行的内容
    int lineNumber = 0;
    std::stack<int> brackets;
    while(std::getline(codeStream, line))
    {
        lineNumber++;
        for (auto c : line) 
        {
            if (c == '[') 
            {
                brackets.push(lineNumber);
                // line_number=c.line_number;
            }        
            else if (c == ']') 
            {
                //检测到']'，若栈不为空，则缺少'[',报错
                if (brackets.empty()) 
                {
                    Token token(SYMBOL,"",lineNumber,file_name);
                    errors.push_back({ token, "Unmatched ']' at line " });
                }
            brackets.pop();//'['出栈
            }
        }
    }

    //若全部读取后栈不为空，则缺少']'，报错
    if (!brackets.empty()) 
    {
        Token token(SYMBOL,"",brackets.top(),file_name);
        errors.push_back({ token , "Unmatched '[' at line " });
    }

    return ;
}

void checkBrackets::checkBrace(std::string code,std::string file_name)
{
    std::istringstream codeStream(code);//将code按行分块
    std::string line;//记录每一行的内容
    int lineNumber = 0;
    std::stack<int> brackets;
    while(std::getline(codeStream, line))
    {
        lineNumber++;
        for (auto c : line) 
        {
            if (c == '{') 
            {
                brackets.push(lineNumber);
                // line_number=c.line_number;
            }        
            else if (c == '}') 
            {
                //检测到'}'，若栈不为空，则缺少'{',报错
                if (brackets.empty()) 
                {
                    Token token(SYMBOL,"",lineNumber,file_name);
                    errors.push_back({ token, "Unmatched '}' at line " });
                }
            brackets.pop();//'{'出栈
            }
        }
    }

    //若全部读取后栈不为空，则缺少'}'，报错
    if (!brackets.empty()) 
    {
        Token token(SYMBOL,"",brackets.top(),file_name);
        errors.push_back({ token , "Unmatched '{' at line " });
    }

    return ;
}

void CheckSemicolon::checkCode(const std::string& code,std::string file_name) 
{
    std::istringstream codeStream(code);//将code按行分块
    std::string line;
    //定义nextline记录下一行的内容，便于略过(){}结构中间的;
    std::string nextline;
    int lineNumber = 0;

    //getline读入codeStream流，默认按换行符分块，并存入line中 
    std::getline(codeStream, line);
    while (std::getline(codeStream, nextline)) 
    {
        lineNumber++;
        checkLine(line,nextline,lineNumber,file_name);
        line=nextline;//line重新赋值为下一行，便于下一次检查
    }
    //因为line最后一行没有检查，所以单独检查
    lineNumber++;
    checkLine(line,"",lineNumber,file_name);
}

void CheckSemicolon::checkLine(const std::string& line,const std::string& nextline, int lineNumber,std::string file_name) 
{
    std::string trimmedLine = trim(line);//去掉收尾空白符后的line
    std::string trimmedNextLine = trim(nextline);//去掉收尾空白符后的nextline
    // 忽略空行和仅有 '{' 或 '}' 开头的块
    if (trimmedLine.empty() || trimmedLine == "{" || trimmedLine == "}") {
        return;
    }

    // 忽略{}结构前的";"检查
    if(trimmedNextLine.front() == '{') 
    {
        return;
    }

    // 如果行没有以分号结尾且不是"{"结尾，报错
    if (!endsWithSemicolon(trimmedLine) && !isBlockStructure(trimmedLine)) {
        //新建一个默认Token，记录错误的行号和文件名，用于报错
        Token token(SYMBOL,"",lineNumber,file_name);
        errors.push_back({token, "Missing semicolon at the end of line "});
    }
    
}

bool CheckSemicolon::endsWithSemicolon(const std::string& line) 
{
    return !line.empty() && line.back() == ';';
}

bool CheckSemicolon::isBlockStructure(const std::string& line) 
{
    return line.back()=='{';
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
            std::cerr << "\033[31m Error (⊙ _⊙ )!!! : " << error.message << error.token.line_number << "\033[0m" << std::endl;
            std::cout << "In file: " << error.token.file_name << " at line: " << error.token.line_number << std::endl;
            IO io(error.token.file_name,"");
            std::cout << "\t \033[33m" << io.read_line(error.token.line_number) << "\033[0m" << std::endl;
        }
        exit(1);//结束程序
    }
}