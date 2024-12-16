#include <bits/stdc++.h>

#ifndef WHUPstream_H
#define WHUPstream_H

class WHUPstream :public std::ostream , std::streambuf{
    public:
    WHUPstream();
    int overflow(int c);
    virtual void out(char c) = 0;
};

class WHUPstream_compile1 :public WHUPstream{
    public:
    WHUPstream_compile1();
    void out(char c);
};

class WHUPstream_compile2 :public WHUPstream{
    public:
    WHUPstream_compile2();
    void out(char c);
};

class WHUPstream_execute :public WHUPstream{
    public:
    WHUPstream_execute();
    void out(char c);
};

class WHUPstream_number{
    std::string buffer;

    public:
    friend WHUPstream_number& operator<<(WHUPstream_number& formatter, float value) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(10) << value; // 固定为非科学计数法
        std::string str = oss.str();
        // 去掉多余的尾随0
        str.erase(str.find_last_not_of('0') + 1);
        if (str.back() == '.') {
            str.pop_back(); // 删除尾部小数点
        }
        formatter.buffer += str + " "; // 缓存结果，用空格分隔多个输入
        std::cout << formatter.buffer;

        formatter.buffer.clear(); // 清空缓存

        return formatter;//沟槽的，非得返回一个，不然会卡住
    }
};
#endif