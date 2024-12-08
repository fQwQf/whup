#include "block.h"

class Arr
{
private:
    Environment* env;
    int size;  
    std::string type;
    std::string name;
    std::vector<std::string> value;
    //“声明”函数，，将新声明的变量加入符号表中
    void declare();

    public:
    Arr(std::vector<Token> tokens,Environment* env);

    void arr(Environment* env, std::string name,int size);

    void initArr(std::vector<Token> code, Environment* env);
};