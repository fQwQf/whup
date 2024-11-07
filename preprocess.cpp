//这个文件定义了预处理函数，可以去掉注释，多余的空格，换行符等
//单行注释由//开头，多行注释由/*开头，*/结尾。

#include <iostream>
#include <string>
#include <sstream>

std::string preprocess(const std::string& code) {
    std::string result;
    bool inBlockComment = false;
    bool inLineComment = false;

    for (size_t i = 0; i < code.size(); ++i) {
        if (inBlockComment) {
            if (code[i] == '*' && i + 1 < code.size() && code[i + 1] == '/') {
                inBlockComment = false;
                i++;
            }
        } else if (inLineComment) {
            if (code[i] == '\n') {
                inLineComment = false;
                //result += code[i];
            }
        } else {
            if (code[i] == '/' && i + 1 < code.size()) {
                if (code[i + 1] == '/') {
                    inLineComment = true;
                    i++;
                } else if (code[i + 1] == '*') {
                    inBlockComment = true;
                    i++;
                } else {
                    result += code[i];
                }
            } else if (code[i] == ' ' || code[i] == '\t') {
                if (!result.empty() && result.back() != ' ' && result.back() != '\n') {
                    result += code[i];
                }
            } else if (code[i] != '\n'){
                result += code[i];
            }
        }
    }

    // 去除末尾多余的空格
    while (!result.empty() && (result.back() == ' ' || result.back() == '\t')) {
        result.pop_back();
    }

    return result;
}