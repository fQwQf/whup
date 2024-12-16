#include "block.h"

class Arr
{
private:
    Environment* env;
      
    std::string type;
    
    std::vector<std::string> value;
    
    //“声明”函数，，将新声明的变量加入符号表中(传入tokens是为了方便报错）
    void declare(std::vector<Token> tokens);

public:
    //记录数组的大小
    int size; 
    //记录数组每个维度的长度
    std::vector<int> len;
    //记录数组的维度
    int dimension;
    std::string name;

    Arr(std::vector<Token> tokens,Environment* env,std::string type);

    void arr(Environment *env, std::string name, std::vector<Token> tokens ,int dimension,int size);

    void initArr(std::vector<Token> code, Environment* env);
};