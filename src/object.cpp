#include"object.h"

//跳过大括号
void Object::matchBrace(int &i,std::vector<Token> &tokens)
{
    if (tokens[i].value == "{")
    {
        int leftPar = 1;
        int rightPar = 0; // 分别记录已经读取的左大括号右大括号的个数,当相等时即可结束
        while (leftPar != rightPar)
        {
            ++i;
            if (tokens[i].value == "}")
            {
                rightPar++;
            }
            else if (tokens[i].value == "{")
            {
                leftPar++;
            }
            else
                continue;
        }
    }
}


//从识别到private,public,protected的部分开始,一直到下一个private,public,protected,或者到类声明结尾为一个”block“

void Object::addressBlock(std::vector<Token>tokens,LimitsType type)
{
    int last_semicolon = 0;
    for (int i = 0; i < tokens.size(); i++)
    {
        matchBrace(i, tokens);
        if (tokens[i].type == SYMBOL && tokens[i].value == ";")
        {
            std::vector<Token> subtokens(tokens.begin() + last_semicolon, tokens.begin() + i);
            last_semicolon = i+1;
            generateVar(subtokens,type);
        }
    }
}


Object::Object(std::vector<Token>tokens)
{
    for(int i=0;i<tokens.size();i++)
    {
        if(tokens[i].value=="private")
        {
            i++;
            if(tokens[i].value!=":")//做一个简单的语法检查
                std::cout<<"need a :"<<std::endl;
            i++;
            LimitsType type=PRIVATE;
            int blockBegin=i;
            for(;i<tokens.size();i++)
            {
                if(tokens[i].value=="protected"||tokens[i].value=="public")//找到下一个板块的位置
                {
                    break;
                }
            }
            addressBlock(std::vector<Token>(tokens.begin()+blockBegin,tokens.begin()+i),type);
        }
        else if(tokens[i].value=="protected")
        {
            i++;
            if(tokens[i].value!=":")//做一个简单的语法检查
                std::cout<<"need a :"<<std::endl;
            i++;
            LimitsType type=PROTECTED;
            int blockBegin=i;
            for(;i<tokens.size();i++)
            {
                if(tokens[i].value=="private"||tokens[i].value=="public")//找到下一个板块的位置
                {
                    break;
                }
            }
            addressBlock(std::vector<Token>(tokens.begin()+blockBegin,tokens.begin()+i),type);
        }
        else if(tokens[i].value=="public")
        {
            i++;
            if(tokens[i].value!=":")//做一个简单的语法检查
                std::cout<<"need a :"<<std::endl;
            i++;
            LimitsType type=PUBLIC;
            int blockBegin=i;
            for(;i<tokens.size();i++)
            {
                if(tokens[i].value=="private"||tokens[i].value=="protected")//找到下一个板块的位置
                {
                    break;
                }
            }
            addressBlock(std::vector<Token>(tokens.begin()+blockBegin,tokens.begin()+i),type);
        }
    }
}