#include "function.h"

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

extern std::unordered_map<std::string, std::vector<std::string>> functions;  // 存储函数名和形参对应的哈希表
extern std::vector<ThreeAddressCode> function_tacs; // 存储函数内三地址代码的向量
extern std::unordered_map<std::string, std::string> var_declares;  // 存储将放入c++中变量名和类型的哈希表


//这是对函数声明的识别，会将函数名与参数存入哈希表，并且将函数体压入function_tacs
class Function {
    std::string name;
    std::string return_type;
    std::string body;
    std::vector<std::string> params_name;
    std::vector<std::string> params_type;

    public:
        Function(std::vector<Token> &tokens) {
            tokens.erase(tokens.begin());//删除第一个token，即function关键字
            name = tokens[0].value;
            tokens.erase(tokens.begin());

            //现在开始分析形参
            //TODO:如果这里发现错误如首token不是括号，抛出异常
            if (tokens[0].type == SYMBOL && tokens[0].value == "(") {
                tokens.erase(tokens.begin());
                //接下来要按照逗号和冒号来分割参数
                while (tokens[0].type != SYMBOL || tokens[0].value != ")") {
                    params_name.push_back(tokens[0].value);
                    tokens.erase(tokens.begin());
                    if (tokens[0].type == SYMBOL && tokens[0].value == ",") {
                        params_type.push_back("auto");
                        tokens.erase(tokens.begin());
                    } else if (tokens[0].type == SYMBOL && tokens[0].value == ":") {
                        tokens.erase(tokens.begin());
                        params_type.push_back(tokens[0].value);
                        tokens.erase(tokens.begin(),tokens.begin()+1);
                    }
                }
                tokens.erase(tokens.begin());
            }

            //这里分析返回类型
            if (tokens[0].type == IDENTIFIER && tokens[0].value == ":") {
                tokens.erase(tokens.begin());
                return_type = tokens[0].value;
                tokens.erase(tokens.begin());
            }else{
                return_type = "auto";
            }

            //这里分析函数体
            //函数体分析可以直接继承Block,因为函数体就是一段代码块
            //只需要对Token进行修改，改变其中的参数名即可


        }
        
};

class FunctionBlock : public Block{




};