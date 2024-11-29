# WHUP

![WHUP](logo2.png)
WHUP/ ˈdʌbljuː eɪtʃ ʌp /(WHUP: High-level Unified Programming)是门一种简洁的，拥有先编译后执行特性的强类型，静态类型编程语言。在这里，我们将实现能将其能编译成一种类似于汇编的，供虚拟机运行的中间代码HUST的编译器；以及一个模拟没有寄存器且堆可以直接通过变量名访问的计算机的虚拟机，运行HUST。  

## 关于logo

logo左边的图形是w和u的结合，右边的图形由左边的旋转180°而来，同时也是h和p的结合。向上的箭头指的是whup里的"up"。同时该logo的形状像两个回形针，暗示了whup的语法如同用回形针固定文件一样，模块化程度高，使其可拓展性强。  

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

## 一点想法  

我们可以先执行语法分析，再执行词法分析。  
作用域可以以再变量名后面加上作用域的编号来区分。当然，这个需要分析作用域，而我觉得生成语法树之后再分析简单一些——所以可以先执行语法分析，再执行词法分析。  
在优化语法树之后，可以将语法树以中间代码的形式输出，便于跨平台。实际上这借鉴了Java的编译原理。  

结合性规则规定了同一运算符多次出现时的解析规则。`+`,`-`,`*`,`/`,`%`等运算符是左结合的，而=是右结合的。`==`,`!=`,`>`,`<`,`>=`,`<=`是左结合的，但我们也可以不允许这些符号在一个表达式中多次出现。  
类似的，优先级规则规定了不同运算符的解析规则。  
优先级高的后解析，优先级低的先解析。  
左结合的从右往左解析，右结合的从左往右解析。  
对于结合性和优先级，直接借鉴其他语言即可。  

表达式：如：`(a+b)*c`，实际上有返回值。放在括号里的式子都是表达式。  
表达式不一定要用树状结构表示，也可以用后缀表达式表示。  

语句列表：由语句组成的列表，如：`a=1;b=2;`。放在大括号里的语句都是语句列表。语句列表可以根据分号分为多条语句。语句可以是表达式，也可以是内置函数调用，如：print(a+b)。  

内置函数：如：`print(a+b)`。  

声明：我们用关键字`var`来声明变量。如：

```Javascript
var a=1;
```

用关键字`function`来声明函数。如：

```Javascript
function add(a,b){
    return a+b;
};
```

用关键字`class`来声明类。如：

```Javascript
class A{
    var a=1;
    function add(a,b){
        return a+b;
    };
};
```  

## 核心议题

- 可以引入动态类型检查吗？如何抛出错误？
- 如何设计预处理器来链接各部分的源代码？

## whup 语法规则

请参考[grammar.md](/docs/grammar.md)。
