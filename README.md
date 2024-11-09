# WHUP

![WHUP](logo2.png)
WHUP/ ˈdʌbljuː eɪtʃ ʌp /(WHUP: High-level Unified Programming)是一个强类型，动态类型的编程语言。在这里，我们将实现whup的解释器，如果可能的话，实现whup的编译器。  

## 关于logo

logo左边的图形是w和u的结合，右边的图形由左边的旋转180°而来，同时也是h和p的结合。向上的箭头指的是whup里的"up"。同时该logo的形状像两个回形针，暗示了whup的语法如同用回形针固定文件一样，模块化程度高，使其可拓展性强。  

## 重要数据结构

### 1.抽象语法树

抽象语法树是whup程序的树形结构。它由节点组成，每个节点代表一个表达式，语句或声明。

#### 结构

对于一个节点，它应该有：

- 一个构造函数，能抛出异常，执行类型转换
- 一个生成中间简化c++的方法
- 一个判断子节点类型，并创建相应子节点的方法

### 2.符号表

符号表是whup程序中标识符的集合。它存储标识符的名称，类型和值。  
出于处理变量作用域的考虑，符号表应当是树状结构。每个作用域都是一个节点。  
由于c++是静态类型语言，所以如果一个变量的类型被修改，那么对应的c++变量应当改变。为了简便起见，我们可以将转化后的c++变量命名为原变量名+类型+作用域编号的形式。  

## 重要组件

### 1. 词法分析器

词法分析器将输入的字符串中的词素转化为词法单元。词素是单词，符号，数字等。  
变量作用域在词法分析器中处理，具体方法是对不同作用域的变量名在词法分析中进行区分。  
词法分析器输出结果应包含代码所在行，便于调试。  

### 2. 语法分析器

语法分析器通过递归下降算法将词法单元转化为抽象语法树。  
在生成语法树时，应该为所有表达式的返回值创建一个临时变量，便于生成三字节码。  
对于终结符，应该特殊考虑。如变量名，字符串，数字，布尔值等。
赋值号的左值应该是变量，而不是表达式。
符号表会在语法分析器中建立。

### 3. 语义分析器

语义分析器负责检查抽象语法树中的语义错误，如变量未定义，类型不匹配等。同时进行自动类型转换。

### 4. 代码优化器

代码优化器负责对抽象语法树进行优化，如常量折叠，常量传播等。（可以先不实现）

### 5. 中间简化c++生成器

中间简化c++生成器负责将抽象语法树转化为中间简化c++。  
中间简化c++是仅有类型转换，定义，与基础运算，goto,简单的单行if的c++。这样便于转化为汇编。  

### 6.2 whupvm虚拟机

执行器负责执行中间简化c++。

## 一点想法  

我们可以先执行语法分析，再执行词法分析。  
作用域可以以再变量名后面加上作用域的编号来区分。当然，这个需要分析作用域，而我觉得生成语法树之后再分析简单一些——所以可以先执行语法分析，再执行词法分析。  
在优化语法树之后，可以将语法树以中间代码的形式输出，便于跨平台。实际上这借鉴了Java的编译原理。  
代码生成器可以编译为c++，再编译为可执行文件。  
执行器可以从json中读取语法树，再执行。也就是说，这个json起到了类似于java的class的作用。  

结合性规则规定了同一运算符多次出现时的解析规则。+,-,*,/,%等运算符是左结合的，而=是右结合的。==,!=,>,<,>=,<=是左结合的，但我们也可以不允许这些符号在一个表达式中多次出现。  
类似的，优先级规则规定了不同运算符的解析规则。  
优先级高的后解析，优先级低的先解析。  
左结合的从右往左解析，右结合的从左往右解析。  
对于结合性和优先级，直接借鉴其他语言即可。  

表达式：如：(a+b)*c，实际上有返回值。放在括号里的式子都是表达式。  
表达式不一定要用树状结构表示，也可以用后缀表达式表示。  

语句列表：由语句组成的列表，如：a=1;b=2;。放在大括号里的语句都是语句列表。语句列表可以根据分号分为多条语句。语句可以是表达式，也可以是内置函数调用，如：print(a+b)。  

内置函数：如：print(a+b)。  

声明：我们用关键字var 来声明变量。如：var a=1;。用关键字func 来声明函数。如：func add(a,b){return a+b;}。用关键字class 来声明类。如：class A{var a=1;func add(a,b){return a+b;}}。  

## 核心议题

- 是否真的构建一颗抽象语法树？    
- 词法分析传出结果形式？  
- 语法分析器具体如何处理作用域？  
- 函数和类在哪里处理？  
- 中间代码形式？  
- 静态类型检查在哪里，怎样进行？  
- 可以引入动态类型检查吗？如何抛出错误？  