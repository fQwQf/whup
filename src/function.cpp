#include "function.h"
#include "expression.h"

/*
## 函数

函数就是包裹在花括号中的代码块，前面使用了关键词 function：

```Javascript
function functionname()
{
    // 执行代码
};
```

当调用该函数时，会执行函数内的代码。

可以在某事件发生时直接调用函数（比如当用户点击按钮时），并且可由 whup 在任何位置进行调用。

whup 对大小写敏感。关键词 function 必须是小写的，并且必须以与函数名称相同的大小写来调用函数。

调用带参数的函数
在调用函数时，您可以向其传递值，这些值被称为参数。
这些参数可以在函数中使用。
您可以发送任意多的参数，由逗号 (,) 分隔：

```JavaScript
myFunction(argument1,argument2);
```

当您声明函数时，请把参数作为变量来声明：

```JavaScript
function myFunction(var1,var2){
    代码
};
```

变量和参数必须以一致的顺序出现。第一个变量就是第一个被传递的参数的给定的值，以此类推。
您可以在形参中不指定参数类型，此时whup会自动分析参数类型。但这可能会造成一些意外结果，如：

```JavaScript
function myFunction(var1,var2){
    print(var1 + var2);
};
myFunction("hello", 3);  // 异常
myFunction(3, 4);  // 输出 7
myFunction(3, 4.5);  // 输出 7.5
myFunction(3, true);  // 输出 4
```

这是由于whup返回值类型并非可变，在未声明的情况下由whup分析并确定。一旦出现类型不匹配，whup会尝试进行类型转换，然而类型转换并不一定成功。在上面的例子中，whup自动将返回值确定为数字，并会尝试将字符串转换为数字，但是由于字符串"hello3"无法转换为数字，因此无法运行。
**注意！**这种错误是无法获到的，因为whup在运行时才进行类型转换，而函数调用是在编译时就已确定。因此，在涉及字符串与数字的混合处理时，指定类型是一个良好的习惯。
请按以下语法来避免类型转换的错误：

```JavaScript
function myFunction(var1: string, var2: number){
    print(var1 + string(var2));
};
myFunction("hello", 3);  //输出 hello3
```

另外，whup 也支持函数返回值类型声明，如下例所示。

```JavaScript
function myFunction(var1: string, var2: number): string{
    return var1 + string(var2);
};
```

值得一提的是，whup 支持函数重载，即函数名相同但参数类型不同的函数可以并存。另外，whup 支持函数默认参数，即函数调用时可以不传递参数，此时参数将使用默认值。并且，重复的重载将被覆盖。

```JavaScript
function myFunction(var1: str, var2: int = 3){
    print(var1 + str(var2));
};
function myFunction(var1: int, var2: int = 3){
    print(var1 + var2);
};
myFunction("hello",);  // 输出 hello3
myFunction(3,);  // 输出 6
```

使用默认参数时应该注意：如果函数有默认参数，那么在调用函数时，必须以留空的形式提供所有非默认参数，否则会报错。不过，默认参数的位置是灵活的，不必全部放在最后。如：

```JavaScript
function myFunction(var1: int, var2: int = 3, var3: int){};
myFunction(1,2,3);  // 正确
myFunction(1,2);  // 错误
myFunction(1, ,2);  // 正确
myFunction(1,);  // 错误
```
*/

std::string function_ret_label;//只有在处理函数时才会有的值。用于函数返回时跳转至ret区域。

extern std::unordered_map<std::string, Function*> functions;  // 存储函数名和对应的对象指针哈希表
extern std::unordered_map<std::string, std::string> var_declares;  // 存储将放入c++中变量名和类型的哈希表
extern std::vector<ThreeAddressCode> tacs; // 存储三地址代码的向量


Function::Function(std::vector<Token> &tokens,Environment *env)
{
    start_label = newTempLabel();
    end_label = newTempLabel();
    jump_in_label = newTempVar("string");
    this->env = env;

    tokens.erase(tokens.begin()); // 删除第一个token，即function关键字
    name = tokens[0].value;
    tokens.erase(tokens.begin());

    functions[name] = this;

    // 现在开始分析形参
    // TODO:如果这里发现错误如首token不是括号，抛出异常
    if (tokens[0].type == SYMBOL && tokens[0].value == "(")
    {
        tokens.erase(tokens.begin());
        // 接下来要按照逗号和冒号来分割参数
        while (tokens[0].type != SYMBOL || tokens[0].value != ")")
        {
            params_name.push_back({tokens[0].value, ""});
            tokens.erase(tokens.begin());
            if (tokens[0].type == SYMBOL && tokens[0].value == ",")
            {
                params_type.push_back("auto");
                params_name.back().second = newTempVar("auto");
                tokens.erase(tokens.begin());
            }
            else if (tokens[0].type == SYMBOL && tokens[0].value == ":")
            {
                tokens.erase(tokens.begin());
                params_type.push_back(tokens[0].value);
                params_name.back().second = newTempVar(tokens[0].value);

                tokens.erase(tokens.begin(), tokens.begin() + 1);
            }
        }
        tokens.erase(tokens.begin());
    }

    // 这里分析返回类型
    if (tokens[0].type == IDENTIFIER && tokens[0].value == ":")
    {
        tokens.erase(tokens.begin());
        return_type = tokens[0].value;
        return_value = newTempVar(return_type);
        tokens.erase(tokens.begin());
    }
    else
    {
        return_type = "auto";
    }

    // 这里分析函数体
    // 函数体分析可以直接继承Block,因为函数体就是一段代码块
    // 只需要对Token进行修改，改变其中的参数名即可，改成专用参数名
    for (auto &token : tokens)
    {
        if (token.type == IDENTIFIER)
        {
            for (int i = 0; i < params_name.size(); i++)
            {
                if (token.value == params_name[i].first && token.type == IDENTIFIER)
                {
                    token.value = params_name[i].second;
                    token.processed = true;
                    break;
                }
            }
        }
    }

    this->body_tokens = tokens;
}

void Function::matchPar(int &i,std::vector<Token> &tokens)//实际上是由marchBrace改过来的
{
    if (tokens[i].value == "(")
    {
        int leftPar = 1;
        int rightPar = 0; // 分别记录已经读取的左大括号右大括号的个数,当相等时即可结束
        while (leftPar != rightPar)
        {
            ++i;
            if (tokens[i].value == ")")
            {
                rightPar++;
            }
            else if (tokens[i].value == "(")
            {
                leftPar++;
            }
            else
                continue;
        }
    }
}


std::string Function::call(std::vector<Token> &tokens,Environment* env){//返回值是储存返回值的临时变量名
    tokens.erase(tokens.begin());//去掉函数名

    //现在开始处理参数，具体来说，根据逗号，将参数分为多个subtokens，然后传入expression，最后将结果赋给形参
    //实际上这一块和block扫描statement的逻辑类似，所以我直接把block拿过来修改一下就成了🙂‍↕️
    int last_comma = 0;
    int param_num = 0;
    tokens.erase(tokens.begin());
    for (int i = 0; i < tokens.size(); i++)
    {
        matchPar(i, tokens);
        if (tokens[i].type == SYMBOL && (tokens[i].value == "," || tokens[i].value == ")"))
        {
            std::vector<Token> subtokens(tokens.begin() + last_comma, tokens.begin() + i);
            last_comma = i+1;
            Expr* expression = new Expr(subtokens,env);
            tacs.push_back({"=",expression->getTacResult(),"",params_name[param_num].second});
            param_num++;
        }
        if (tokens[i].type == SYMBOL && tokens[i].value == ")")
        {
            tokens.erase(tokens.begin(),tokens.begin() + i);//检测到括号，则删除括号及括号之前的所有内容
            break;
        }
    }

    //现在应该设置跳转，即一个跳出的if...goto...，一个用于跳回的label
    std::string label = newTempLabel();
    tacs.push_back({"=","\""+label+"\"","",jump_in_label});
    tacs.push_back({"if_goto","true","",start_label});
    tacs.push_back({"label","","",label});
    return_labels.push_back(label);

    return return_value;


}

void Function::generate(){
    function_ret_label = end_label;
    tacs.push_back({"label","","",start_label});
    new Block(body_tokens,env);

    //以下是跳转区
    tacs.push_back({"label","","",end_label});
    for(auto i:return_labels){
        std::string bool_var = newTempVar("bool");
        tacs.push_back({"==",jump_in_label,"\""+i+"\"",bool_var});
        tacs.push_back({"if_goto",bool_var,"",i});
    }
}

std::string Function::get_return_value(){
    return return_value;
}

/*class FunctionBlock : public Block{
    Environment* env;//在全局Environment声明函数，函数所在Block的环境就是全局环境;对象内同理
    

    public:
    FunctionBlock(std::vector<Token> tokens,Environment* env){
        this->env = env;
        function_blocks.push_back(this);
    };

};*/