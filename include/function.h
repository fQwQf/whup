#include <bits/stdc++.h>
#include "whup_parser.h"
#include "block.h"

//这是对函数声明的识别，会将函数名与参数存入哈希表，并且将函数体压入function_tacs
class Function {
    std::string name;
    
    std::string body;
    std::vector<std::pair<std::string,std::string>> params_name;//形参名,分别为原形参名和生成的专用形参名
    std::vector<std::string> params_type;//形参类型
    std::string end_label; //函数结束标签
    std::string start_label; //函数开始标签

    std::vector<Token> body_tokens;//函数体

    std::string return_value; //储存返回值的临时变量

    Environment* env;//必然是全局的

    public:
        std::string return_type;

        Function(std::vector<Token> &tokens,Environment* env);
        std::string call(std::vector<Token> &tokens,Environment* env);
        void matchPar(int &i,std::vector<Token> &tokens);
        void matchBrace(int &i,std::vector<Token>&tokens);
        void generate();//生成函数代码
        std::string get_return_value();

        //下面拟进行模块化处理
        void folmalPara(std::vector<Token>&tokens);
        void returnType(std::vector<Token>&tokens);
        void bodyTokens(std::vector<Token>&tokens);
        void realPara(std::vector<Token>&tokens,Environment*env);

};


