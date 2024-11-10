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

## whup 语法规则

whup 的语法借鉴了 Python 和 JavaScript，设计简洁易懂。

### whup 变量

#### 声明（创建）变量

在 whup 中，创建变量通常称为“声明”变量。  

使用 `var` 关键字声明变量：  

```JavaScript
var name;
```

声明后，变量初始为空（未赋值）。可通过 `=` 进行赋值：

```JavaScript
name = "fQwQf";
```

也可以在声明时直接赋值：

```JavaScript
var name = "fQwQf";
```

whup 会在初次赋值时自动确定变量的数据类型。  
此外，也可以直接赋值而不使用 `var`，whup 会自动声明变量，但在变量作用域上可能产生意料外的结果。

#### whup 的动态类型

whup 具有动态类型，这意味着同一个变量可以在不同时候表示不同的类型：

```JavaScript
var x;               // x 为 undefined
x = 5;               // 现在 x 是整数
x = "John";          // 现在 x 是字符串
```

可以使用 `type()` 函数查看变量的数据类型。

#### 数据类型转换

在 whup 中，数据类型转换分为隐式和显式两种。

- **隐式类型转换** - 自动完成
- **显式类型转换** - 使用类型转换函数

##### 隐式类型转换

whup 在隐式类型转换中会自动将一种类型的数据转换为另一种类型。  
如在以下示例中，不同类型的数值运算时会自动将较低类型（整数）转换为较高类型（浮点数），以防数据丢失。

```JavaScript
var num_int = 123;
var num_flo = 1.23;

var num_new = num_int + num_flo;

print("num_int 数据类型为: " + type(num_int));
print("num_flo 数据类型为: " + type(num_flo));

print("num_new 值为: " + num_new);
print("num_new 数据类型为: " + type(num_new));
```

输出结果为：

```bash
num_int 数据类型为: int
num_flo 数据类型为: float
num_new 值为: 124.23
num_new 数据类型为: float
```

例如，当整型数据与字符串类型的数据进行运算时，whup 会将整型数据转换为字符串：

```JavaScript
var num_int = 123;
var num_str = "456";

print("num_int 数据类型为: " + type(num_int));
print("num_str 数据类型为: " + type(num_str));

print(num_int + num_str);
```

输出为：

```bash
num_int 数据类型为: int
num_str 数据类型为: str
123456
```

对于这些情况，whup 提供显式类型转换方法，确保结果符合预期。

##### 显式类型转换

通过显式类型转换，用户可以将数据类型转换为所需类型。使用 `int()`、`float()`、`str()` 等函数完成显式转换：

**int() 转换为整型**

```JavaScript
var x = int(1);    // x 为 1
var y = int(2.8);  // y 为 2
var z = int("3");  // z 为 3
```

**float() 转换为浮点型**

```JavaScript
var x = float(1);      // x 为 1.0
var y = float(2.8);    // y 为 2.8
var z = float("3");    // z 为 3.0
var w = float("4.2");  // w 为 4.2
```

**str() 转换为字符串类型**

```JavaScript
var x = str("s1");  // x 为 's1'
var y = str(2);     // y 为 '2'
var z = str(3.0);   // z 为 '3.0'
```

可用强制类型转换完成整型和字符串类型的运算：

```JavaScript
var num_int = 123;
var num_str = "456";

print("num_int 数据类型为: " + type(num_int));
print("转换前，num_str 数据类型为: " + type(num_str));

num_str = int(num_str);   // 强制转换为整型
print("转换后，num_str 数据类型为: " + type(num_str));

var num_sum = num_int + num_str;

print("num_int 与 num_str 相加结果为: " + num_sum);
print("num_sum 数据类型为: " + type(num_sum));
```

输出为：

```bash
num_int 数据类型为: int
转换前，num_str 数据类型为: str
转换后，num_str 数据类型为: int
num_int 与 num_str 相加结果为: 579
num_sum 数据类型为: int
```

#### 变量命名规则

- 变量名必须以字母或下划线 (`_`) 开头。  
- 变量名称区分大小写（如 `y` 和 `Y` 是不同的变量）。  

#### 重新声明变量

在 whup 中，重新声明变量不会覆盖其原有值：

```JavaScript
var name = "fQwQf";
var name;
```

以上代码执行后，`name` 的值仍为 `"fQwQf"`。

### 注释

whup 支持注释，用于解释代码或增加可读性。  
单行注释以 `//` 开头，  
多行注释以 `/*` 开头，以 `*/` 结尾。

### 运算符

#### 运算符优先级

以下为运算符从高到低的优先级，优先级相同的运算符按从左至右的顺序进行运算，除非特别说明：

运算符 | 描述
---|---
(expressions...) | 圆括号的表达式
x[index], x[index:index], x(arguments...), x.attribute | 读取，切片，调用，属性引用
** | 幂运算
*, /, % | 乘，除，取余
+, - | 加和减
<, <=, >, >=, !=, == | 比较运算，包括成员检测和标识号检测
not x, !x | 逻辑非 NOT
and, && | 逻辑与 AND
or, &#124;&#124; | 逻辑或 OR
=, +=, -=, *=, /=, %= | 赋值表达式

#### 字符串的 `+` 运算符

`+` 用于连接文本值或字符串变量。

```JavaScript
txt1 = "What a very";
txt2 = "nice day";
txt3 = txt1 + txt2;
```

此时，`txt3` 为 `"What a verynice day"`。

#### 字符串和数字的加法运算

两个数字相加返回数值和；数值和字符串相加时返回字符串：

```JavaScript
x = 5 + 5;
y = "5" + 5;
z = "Hello" + 5;
```

输出：

```JavaScript
10
55
Hello5
```

### 作用域

在 whup 中，作用域指的是变量、对象、函数的可访问范围。  
每段代码、包括函数、类等都有各自的作用域，而作用域之间是嵌套的。  

#### 父作用域与子作用域的变量

在父作用域中声明的变量在其所有子作用域中都可以访问。

```JavaScript
var name = "fQwQf";
// 此处可以调用 name
function myFunction(){
    // 此处也可以调用 name
}
```

在子作用域中声明的变量仅在该作用域内有效，不会在父作用域中被访问到。

```JavaScript
// 此处不能调用 name
function myFunction(){
    var name = "fQwQf";
    // 此处可以调用 name
}
// 此处不能调用 name
```

#### 重新定义变量

在子作用域中重新定义变量时，修改只在子作用域中生效。

```JavaScript
var x = 10;
// 此处x为10
{
    // 此处x为10
    var x = 2;
    // 此处x为2
}
// 此处x为10
```

### 输入与输出

#### `print()` 函数

`print()` 函数用于在控制台打印输出。  
它只接受一个参数并将其打印到控制台，自动处理类型转换。  
无返回值。

#### `input()` 函数

`input()` 函数用于读取用户输入，返回字符串类型数据：

```JavaScript
var x = input("请输入一些文本");
```

如需其他类型输入，可使用显式类型转换。  
