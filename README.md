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
- 可以引入动态类型检查吗？如何抛出错误？
- 函数和类在哪里处理？

## whup语法规则

whup的语法规则借鉴了python和JavaScript。  

### whup 变量

#### 声明（创建） whup 变量

在 whup 中创建变量通常称为"声明"变量。  

我们使用 var 关键词来声明变量：  

```JavaScript
var carname;  
```

变量声明之后，该变量是空的（它没有值）。  
如需向变量赋值，请使用等号：  

```JavaScript
carname="Volvo";
```

不过，您也可以在声明变量时对其赋值：

```JavaScript
var carname="Volvo";
```

在对声明的变量初次赋值时，whup会自动确定变量数据类型。  
您也可以不使用 var 关键词来声明变量，而直接为变量赋值：whup会自动声明变量。但是，在涉及变量作用域时，这可能会造成意料之外的结果。  

#### whup 动态类型

whup 拥有动态类型。这意味着相同的变量可用作不同的类型：

```JavaScript
var x;               // x 为 undefined
var x = 5;           // 现在 x 为数字
var x = "John";      // 现在 x 为字符串
```

变量的数据类型可以使用 type 函数来查看。

#### whup 数据类型转换

有时候，我们需要对数据内置的类型进行转换，数据类型的转换，一般情况下你只需要将数据类型作为函数名即可。

whup 数据类型转换可以分为两种：

- 隐式类型转换 - 自动完成
- 显式类型转换 - 需要使用类型函数来转换

##### 隐式类型转换

在隐式类型转换中，whup 会自动将一种数据类型转换为另一种数据类型，不需要我们去干预。

以下实例中，我们对两种不同类型的数据进行运算，较低数据类型（整数）就会转换为较高数据类型（浮点数）以避免数据丢失。

```JavaScript
var num_int = 123
var num_flo = 1.23

var num_new = num_int + num_flo

print("num_int 数据类型为:"+type(num_int))
print("num_flo 数据类型为:"+type(num_flo))

print("num_new 值为:"+num_new)
print("num_new 数据类型为:"+type(num_new))
```

以上实例输出结果为：

```bash
num_int 数据类型为: int
num_flo 数据类型为: float
num_new: 值为: 124.23
num_new 数据类型为: float
```

代码解析：

实例中我们对两个不同数据类型的变量 num_int 和 num_flo 进行相加运算，并存储在变量 num_new 中。
然后查看三个变量的数据类型。
在输出结果中，我们看到 num_int 是 整型（integer） ， num_flo 是 浮点型（float）。
同样，新的变量 num_new 是 浮点型（float），这是因为 whup 会将较小的数据类型转换为较大的数据类型，以避免数据丢失。
我们再看一个实例，整型数据与字符串类型的数据进行相加：

```JavaScript
var num_int = 123
var num_str = "456"

print("num_int 数据类型为:"+type(num_int))
print("num_str 数据类型为:"+type(num_str))

print(num_int+num_str)
```

以上实例输出结果为：

```bash
num_int 数据类型为: int
num_str 数据类型为: str
123456
```

从输出中可以看出，整型和字符串类型运算时，whup会将整型类型转换为字符串。  
但是，这种结果不一定是我们期望的，whup为这些类型的情况提供了一种解决方案，称为显式转换。

##### 显式类型转换

在显式类型转换中，用户将对象的数据类型转换为所需的数据类型。 我们使用 int()、float()、str() 等预定义函数来执行显式类型转换。  

int() 强制转换为整型：

```JavaScript
var x = int(1)   // x 输出结果为 1
var y = int(2.8) // y 输出结果为 2
var z = int("3") // z 输出结果为 3
```

float() 强制转换为浮点型：

```JavaScript
var x = float(1)     // x 输出结果为 1.0
var y = float(2.8)   // y 输出结果为 2.8
var z = float("3")   // z 输出结果为 3.0
var w = float("4.2") // w 输出结果为 4.2
```

str() 强制转换为字符串类型：

```JavaScript
var x = str("s1") // x 输出结果为 's1'
var y = str(2)    // y 输出结果为 '2'
var z = str(3.0)  // z 输出结果为 '3.0'
```

整型和字符串类型进行运算，就可以用强制类型转换来完成：

```JavaScript
var num_int = 123
var num_str = "456"

print("num_int 数据类型为:"+type(num_int))
print("类型转换前，num_str 数据类型为:"+type(num_str))

num_str = int(num_str)    // 强制转换为整型
print("类型转换后，num_str 数据类型为:"+type(num_str))

var num_sum = num_int + num_str

print("num_int 与 num_str 相加结果为:"+num_sum)
print("num_sum 数据类型为:"+type(num_sum))
```

以上实例输出结果为：

```bash
num_int 数据类型为: int
类型转换前，num_str 数据类型为: str
类型转换后，num_str 数据类型为: int
num_int 与 num_str 相加结果为: 579
num_sum 数据类型为: int
```

#### 变量命名规则

- 变量必须以字母或下划线（_）开头  
- 变量名称对大小写敏感（y 和 Y 是不同的变量）  

#### 重新声明 whup 变量

如果重新声明 whup 变量，该变量的值不会丢失：  

在以下两条语句执行后，变量 carname 的值依然是 "Volvo"：  

```JavaScript
var carname="Volvo";
var carname;
```

### whup 注释

whup 不会执行注释。  
我们可以添加注释来对 whup 进行解释，或者提高代码的可读性。  
单行注释以 `//` 开头。  
多行注释以 `/*` 开始，以 `*/` 结尾。  

### whup 运算符

#### whup运算符优先级

以下表格列出了从最高到最低优先级的所有运算符， 相同单元格内的运算符具有相同优先级。 运算符均指二元运算，除非特别指出。 相同单元格内的运算符从左至右分组（除了幂运算和赋值是从右至左分组）：

运算符 | 描述
---|---
(expressions...) | 圆括号的表达式
x[index], x[index:index], x(arguments...), x.attribute | 读取，切片，调用，属性引用
**|乘方(指数)
*, /, %|乘，除，取余
+, -|加和减
<, <=, >, >=, !=, ==|比较运算，包括成员检测和标识号检测
not x,!x | 逻辑非 NOT
and,&&|逻辑与 AND
or,\|\||逻辑或 OR
=,+=,-=,*=,/=,%=|赋值表达式

#### 用于字符串的 + 运算符

\+ 运算符用于把文本值或字符串变量加起来（连接起来）。  
如需把两个或多个字符串变量连接起来，请使用 + 运算符。

```JavaScript
txt1="What a very";
txt2="nice day";
txt3=txt1+txt2;
```

txt3 运算结果如下:

```JavaScript
What a verynice day
```

#### 对字符串和数字进行加法运算

两个数字相加，返回数字相加的和，如果数字与字符串相加，返回字符串，如下实例：  

```JavaScript
x=5+5;
y="5"+5;
z="Hello"+5;
```

x,y, 和 z 输出结果为:

```JavaScript
10
55
Hello5
```

如果把数字与字符串相加，结果将成为字符串。

### whup作用域

在 whup 中，作用域为可访问变量，对象，函数的集合。  
whup 中每一个代码块，包括函数，类，内置语句都拥有自身的作用域，并且所有作用域之间呈包含关系。  
显然，作用域的父子关系是递归的。即，父作用域的父作用域也是父作用域，子作用域同理。  

#### 父作用域与子作用域变量的关系

在父作用域声明的变量是父作用域变量，它在该作用域与它的所有子作用域中的任何地方都是可见的。  

```JavaScript
var carname="Volvo";
// 这里可以调用 carname 变量
function myFunction(){
    // 这里也可以调用 carname 变量
}
```

在子作用域内声明的变量是子作用域变量，它在父作用域是不可见的。  

```JavaScript
// 这里不能调用 carname 变量
function myFunction(){
    var carname="Volvo";
    // 这里可以调用 carname 变量
}
// 这里不能调用 carname 变量
```

#### 重新定义变量

在子作用域重新声明变量后，对于重新声明的变量所做的修改只在子作用域内有效。

```JavaScript
var x = 10;
// 这里输出 x 为 10
{
    // 这里输出 x 为 10
    var x = 2;
    // 这里输出 x 为 2
}
// 这里输出 x 为 10
```

### whup 输入和输出

#### print() 函数

print() 内置函数用于向控制台打印输出。它只接受一个参数，即要输出到控制台的文本。如果输入并非字符串，那么会自动进行类型转换。  
没有返回值。  

#### 读取用户输入

input() 内置函数接受一行数据，返回一个字符串。

```JavaScript
var x = input("请输入一些文本");
```

如果想要获取非字符串输入，请使用显式类型转换。
