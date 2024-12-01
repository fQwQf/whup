#include"classfunction.h"
#include"expression.h"
#include"class.h"
extern std::string function_ret_label;//只有在处理函数时才会有的值。用于函数返回时跳转至ret区域。
extern std::string function_return_value;//同理
extern std::vector<ThreeAddressCode> tacs;
extern std::unordered_map<std::string,Class*> class_table;

//处理类函数声明时，需要传入类函数表和类环境
ClassFunction::ClassFunction(std::vector<Token> &tokens,Environment *env,std::unordered_map<std::string,ClassFunction*>& function_table)
{
    start_label = newTempLabel();
    end_label = newTempLabel();
    this->env = new Environment(env);//函数自己的环境,且用类的环境初始化，要用类的环境来处理参数

    int i=0;
    if(tokens[0].type==KEYWORD&&tokens[0].value=="function")//若是function关键字，则跳过
    {
        i++;
    }

    if(tokens[i].type==IDENTIFIER&&tokens[i+1].type==SYMBOL&&tokens[i+1].value=="(")
    {
        name=tokens[i].value;
        if(class_table.find(name)==class_table.end())//因为构造函数与类名相同，所以这里要判断是否是构造函数
            function_table[name] = this;//将函数插入类函数表//这里与普通函数不同，普通函数是插入全局函数表
        i++;
    }

    tokens.erase(tokens.begin(),tokens.begin()+i);

    std::cout << "Function name: " << name << std::endl;

    //1.形参处理    
    folmalPara(tokens);

    //2.分析返回类型
    returnType(tokens);

    //3.分析函数体
    bodyTokens(tokens);
}


//与普通函数相比调用的时候，要多一步对实例名的识别，这样才能找到对应的函数表
//现在希望call对构造函数进行特殊处理，需要在其中添加一些判断语句
std::string ClassFunction::call(std::vector<Token> &tokens,Environment* env){//返回值是储存返回值的临时变量名
    std::cout <<"call function " << name << std::endl;
    for(auto&i:tokens)
    {
        std::cout << i.value << " ";
    }
    std::cout << std::endl;

    int i=0;
    if(tokens[1].type==SYMBOL&&tokens[1].value=="->"){
        i+=3;
    }
    else{
        i+=2;
    }

    //5.实参处理
    int beginRealPara=i;
    matchPar(i,tokens);
    std::vector<Token>realParaTokens=std::vector<Token>(tokens.begin()+beginRealPara,tokens.begin()+i+1);

    this->realPara(realParaTokens,env);

    call_with_stack_frame(env);
    
    std::cout << "call function " << name << " success" << std::endl;

    return return_value;
}

std::string ClassFunction::callInline(std::vector<Token> &tokens,Environment* env){//返回值是储存返回值的临时变量名
    std::cout <<"call function " << name << std::endl;
    for(auto&i:tokens)
    {
        std::cout << i.value << " ";
    }
    std::cout << std::endl;

    int i=0;
    if(tokens[1].type==SYMBOL&&tokens[1].value=="->"){
        i+=3;
    }
    else{
        i+=2;
    }

    //5.实参处理
    int beginRealPara=i;
    matchPar(i,tokens);
    std::vector<Token>realParaTokens=std::vector<Token>(tokens.begin()+beginRealPara,tokens.begin()+i+1);

    this->realPara(realParaTokens,env);

    
    std::cout << "call function " << name << " success" << std::endl;

    return return_value;
}
