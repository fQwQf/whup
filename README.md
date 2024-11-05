# WHUP
WHUP/ ˈdʌbljuː eɪtʃ ʌp /(WHUP: High-level Unified Programming)是一个强类型，动态类型的编程语言。在这里，我们将实现whup的解释器，如果可能的话，实现whup的编译器。  
# 重要数据结构
## 1.抽象语法树
抽象语法树是whup程序的树形结构。它由节点组成，每个节点代表一个表达式，语句或声明。
### 结构
对于一个节点，它有如下属性：
```c++
struct Node
{
    NodeType type;
    std::vector<Node*> children;
};
```
其中，NodeType是一个枚举类型，它表示节点的类型。例如，一个节点可以是表达式，语句或声明。  
children是一个指向子节点的指针数组。  

## 2.符号表条目
符号表条目是符号表中存储的条目。它存储标识符的名称，类型和值。
```c++
struct SymbolTableEntry
{
    std::string name;
    TypeNode* type;
    ValueNode* value;
};
```

## 2.符号表
符号表是whup程序中标识符的集合。它存储标识符的名称，类型和值。

# 重要组件

## 1. 词法分析器
词法分析器将输入的字符串中的词素转化为词法单元。词素是单词，符号，数字等。

## 2. 语法分析器
语法分析器通过递归下降算法将词法单元转化为抽象语法树。

## 3. 语义分析器
语义分析器负责检查抽象语法树中的语义错误，如变量未定义，类型不匹配等。同时进行自动类型转换。 

## 4. 代码优化器
代码优化器负责对抽象语法树进行优化，如常量折叠，常量传播等。（可以先不实现）

## 5.1 代码生成器
代码生成器负责将抽象语法树转化为机器码。（可以先不实现）

## 5.2 执行器
执行器负责按照抽象语法树执行。


# 一点想法  
我们可以先执行语法分析，再执行词法分析。  
在优化语法树之后，可以将语法树以json的形式输出，便于跨平台。  
代码生成器可以编译为c++，再编译为可执行文件。
