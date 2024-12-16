# WHUP

![WHUP](logo2.png)
WHUP/ ˈdʌbljuː eɪtʃ ʌp /(WHUP: High-level Unified Programming)是一个以WHU(Wholesome High-level Utility)编程语言为核心的项目。本项目将实现两种编程语言：WHU和HUST(Highly Usable Symbolic Technology
)，以及几个相应的软件：WHUPC,WHUPVM和WHUPRUN。  
WHU是一种简洁的，拥有先编译后执行特性的强类型，静态类型高级编程语言。  
HUST是一种可供WHUPVM虚拟机直接运行的低级语言。  
WHUPC是能将高级语言WHU编译成低级语言HUST的编译器。  
WHUPVM是一个模拟没有寄存器且堆可以直接通过变量名访问的计算机的虚拟机，可直接运行HUST。  
WHUPRUN是WHUPC和WHUPVM的组合体，可直接运行WHU代码。  

## 关于logo

logo左边的图形是w和u的结合，右边的图形由左边的旋转180°而来，同时也是h和p的结合。向上的箭头指的是whup里的"up"。同时该logo的形状像两个回形针，暗示了whup的两种语言的语法都如同用回形针固定文件一样，模块化程度高，使其可拓展性强。  

## 重要数据结构

### 1.抽象语法树

抽象语法树是whup程序的树形结构。它由节点组成，每个节点代表一个表达式，语句或声明。

#### 结构

对于一个节点，它应该有：

- 一个构造函数，能抛出异常，执行类型转换
- 一个生成TAC的方法
- 一个判断子节点类型，并创建相应子节点的方法

### 2.符号表

符号表是whup程序中标识符的集合。它存储标识符的名称，类型和值。  
出于处理变量作用域的考虑，符号表应当是树状结构。每个作用域都是一个节点。  

## 重要组件

### 1. 词法分析器

词法分析器将输入的字符串中的词素转化为词法单元。词素是单词，符号，数字等。  
变量作用域在词法分析器中处理，具体方法是对不同作用域的变量名在词法分析中进行区分。  
词法分析器输出结果应包含代码所在行，便于调试。  

### 2. 语法分析器

语法分析器通过递归下降算法将词法单元转化为抽象语法树并生成HUST。  
在生成语法树时，应该为所有表达式的返回值创建一个临时变量，便于生成三字节码。  
对于终结符，应该特殊考虑。如变量名，字符串，数字，布尔值等。
赋值号的左值应该是变量，而不是表达式。
符号表会在语法分析器中建立。

### 3. 检查器

语义分析器负责检查各种错误，如变量未定义，类型不匹配等。

### 4. 代码优化器

代码优化器负责对抽象语法树进行优化，如常量折叠，常量传播等。（可以先不实现）

### 5. HUST生成器

HUST生成器负责将TAC转化为HUST。  
HUST是一种类似于汇编的，供虚拟机运行的中间代码。  

### 6.2 whupvm虚拟机

执行器负责执行HUST。

## 核心算法

### 简介

我们在编译器的语法分析阶段使用了递归下降算法，通过递归调用函数来处理语法结构，生成抽象语法树和三地址代码。你的实现包括以下几个方面：

1.为每个语法规则编写相应的递归函数。
2.使用词法分析器将源代码分解成词法单元。
3.通过递归调用函数解析语法结构。
4.在解析过程中检测语法错误并提供错误信息。
5.生成抽象语法树和三地址代码。
这种方法结构清晰，易于理解和维护，适用于处理简单的语法规则。

我们为语句块设计了Block类
Block能够按分号分隔语句并逐句处理
所有语句都在一个全局Block内
抽象语法树中Block的子节点的个数又whup源代码决定

```cpp
//这里给出了部分子节点的处理方式，主要是对关键字的处理
void Block::generate(std::vector<Token> subtokens)
{
     if (subtokens[0].type == KEYWORD && subtokens[0].value == "var")
    {
        new Var(subtokens,env);
        WHUPout << "var generate" << std::endl;
    }
    else if(subtokens[0].type == KEYWORD && subtokens[0].value == "number")
    {
        WHUPout << "find number" << std::endl;
        new Arr(subtokens,env,"number");
        WHUPout << "arr generate" << std::endl;
    }
    else if(subtokens[0].type == KEYWORD && subtokens[0].value == "string")
    {
        new Arr(subtokens,env,"string");
        WHUPout << "arr generate" << std::endl;
    }
    else if (subtokens[0].type == KEYWORD && subtokens[0].value == "print")
    {
        new Print(subtokens,env);
        WHUPout << "print generate" << std::endl;
    }
    else if(subtokens[0].type==KEYWORD && subtokens[0].value=="if")
    {
        new If(subtokens,env);
    }
    else if(subtokens[0].type==KEYWORD && subtokens[0].value=="while")
    {
        new While(subtokens,env);
    }
    else if(subtokens[0].type==KEYWORD && subtokens[0].value=="break")
    {
        new Break(env);
    }
    else if(subtokens[0].type==KEYWORD&&subtokens[0].value=="continue")
    {
        new Continue(env);
    }
    else if(subtokens[0].type==KEYWORD&&subtokens[0].value=="return")
    {
        if (function_ret_label == "")
        {
            pushErrors(subtokens[0],"unexpected retrun");
        }
        else
        {
            if (subtokens.size() == 1)
            {
                new Return(env);
                WHUPout << "find return!" << std::endl;
            }
            else
            {
                new Return(subtokens, env);
                WHUPout << "find return!" << std::endl;
            }
        }
    }
    else if(subtokens[0].type==KEYWORD&&subtokens[0].value=="function")
    {
        new Function(subtokens,env);
    }
    else if(subtokens[0].type==KEYWORD&&subtokens[0].value=="class")
    {
        new Class(subtokens);
    }
}
```

在Block的成员函数generator中我们对单个语句进行判断，调用相应的构造函数
而在各种语法规则的构造函数中，由于存在Block的嵌套，又会递归地调用Block的构造对象

以While循环类为例：
```cpp
class While
{
    //抽象语法树中，一个While结点有一个Expr子节点和一个Block子节点
	Expr* While_Expr;
	Block* While_Block;

	Environment* While_env;
public:
	While(std::vector<Token> &code, Environment* env);
	void matchPar(int& pos, std::vector<Token>code);
	void matchBrace(int& pos, std::vector<Token>code);
};


While::While(std::vector<Token> &code,Environment*env):While_env(env)
{
    //此处省略语句处理
    While_Expr = new Expr(std::vector<Token>(code.begin(), code.begin() + pos+1),this->While_env);
    //此处省略语句处理、跳转实现等操作
    While_Block = new Block(std::vector<Token>(code.begin() + Block_begin + 1, code.begin() + pos+1), While_env);
}
```

### 时间复杂度 

递归下降解析器的时间复杂度主要取决于输入的长度和解析过程中递归调用的深度。对于每个语法规则，解析器会扫描输入的每个词法单元（Token），并根据语法规则进行递归调用。

1.单个语法规则的时间复杂度：
    对于每个语法规则，解析器会扫描输入的每个词法单元，时间复杂度为 O(n)，其中 n 是输入的长度。

2.整体时间复杂度：
    由于递归下降解析器是自顶向下解析的，每个语法规则的解析时间是线性的，因此整体时间复杂度为 O(n)，其中 n 是输入的长度。

### 空间复杂度

递归下降解析器的空间复杂度主要取决于递归调用的深度和解析过程中使用的数据结构。

1.递归调用的深度：
递归调用的深度取决于语法规则的嵌套层次。对于大多数编程语言，语法规则的嵌套层次是有限的，因此递归调用的深度是常数级别的。
例如，在解析嵌套的括号或大括号时，递归调用的深度取决于括号或大括号的嵌套层次。

2.使用的数据结构：
解析器在解析过程中会使用一些数据结构来存储词法单元、抽象语法树和三地址代码等。
这些数据结构的空间复杂度通常是线性的，即 O(n)，其中 n 是输入的长度。

### 代码分析

```cpp
void Block::block(std::vector<Token> tokens) {
    for (int i = 0; i < tokens.size(); ++i) {
        if (tokens[i].type == IDENTIFIER) {
            // 根据首token传入对应的类的构造函数中
            if (tokens[i].value == "if") {
                If ifStatement(tokens, env);
            } else if (tokens[i].value == "while") {
                While whileStatement(tokens, env);
            } else if (tokens[i].value == "return") {
                Return returnStatement(tokens, env);
            }
            // 其他语句类型的处理...
        }
    }
}
```

时间复杂度：block 函数会遍历所有的词法单元，并根据首个词法单元调用相应的类的构造函数来处
理不同的语句类型。因此，时间复杂度为 O(n)，其中 n 是输入的长度。

空间复杂度：block 函数使用了线性级别的空间来存储词法单元，因此空间复杂度为 O(n)。


## 核心议题

- 可以引入动态类型检查吗？如何抛出错误？
- 如何设计预处理器来链接各部分的源代码？

## whup 语法规则

请参考[grammar.md](/docs/grammar.md)。  

## 示例

请参考[examples](/docs/example.md)。
