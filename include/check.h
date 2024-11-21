#include"whupc.h"
#include"block.h"

class check
{
public:
    //检查小括号是否匹配
    bool checkPar(std::vector<Token>code);
    //检查中括号是否匹配
    bool checkBracket(std::vector<Token>code);
    //检查大括号是否匹配
    bool checkBrace(std::vector<Token>code);

private:
};
