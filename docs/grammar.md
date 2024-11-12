# whup 语法规则

whup 的语法借鉴了 Python 和 JavaScript，设计简洁易懂。

## whup 变量

### 声明（创建）变量

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
此外，也可以直接赋值而不使用 `var` ，（目前不实现）whup 会自动声明变量，但在变量作用域上可能产生意料外的结果 。

### whup 的动态类型

whup 具有动态类型，这意味着同一个变量可以在不同时候表示不同的类型：

```JavaScript
var x;               // x 为 undefined
x = 5;               // 现在 x 是整数
x = "John";          // 现在 x 是字符串
```

可以使用 `type()` 函数查看变量的数据类型。

### 数据类型转换

在 whup 中，数据类型转换分为隐式和显式两种。

- **隐式类型转换** - 自动完成
- **显式类型转换** - 使用类型转换函数

#### 隐式类型转换

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

#### 显式类型转换

通过显式类型转换，用户可以将数据类型转换为所需类型。使用 `int()`、`float()`、`str()` 等函数完成显式转换：

- **int() 转换为整型**

```JavaScript
var x = int(1);    // x 为 1
var y = int(2.8);  // y 为 2
var z = int("3");  // z 为 3
```

- **float() 转换为浮点型**

```JavaScript
var x = float(1);      // x 为 1.0
var y = float(2.8);    // y 为 2.8
var z = float("3");    // z 为 3.0
var w = float("4.2");  // w 为 4.2
```

- **str() 转换为字符串类型**

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

### 变量命名规则

- 变量名必须以字母或下划线 (`_`) 开头。  
- 变量名称区分大小写（如 `y` 和 `Y` 是不同的变量）。  

### 重新声明变量

在 whup 中，重新声明变量不会覆盖其原有值：

```JavaScript
var name = "fQwQf";
var name;
```

以上代码执行后，`name` 的值仍为 `"fQwQf"`。

## 注释

whup 支持注释，用于解释代码或增加可读性。  
单行注释以 `//` 开头，  
多行注释以 `/*` 开头，以 `*/` 结尾。

## 运算符

### 运算符优先级

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

### 字符串的 `+` 运算符

`+` 用于连接文本值或字符串变量。

```JavaScript
txt1 = "What a very";
txt2 = "nice day";
txt3 = txt1 + txt2;
```

此时，`txt3` 为 `"What a verynice day"`。

### 字符串和数字的加法运算

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

请注意，这只是whup对于加号的特别处理，仅仅是一个特例，这并不代表whup是弱类型语言。  

## 作用域

在 whup 中，作用域指的是变量、对象、函数的可访问范围。  
每段代码、包括函数、类等都有各自的作用域，而作用域之间是嵌套的。  

### 父作用域与子作用域的变量

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

### 重新定义变量

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

## 输入与输出

### `print()` 函数

`print()` 函数用于在控制台打印输出。  
它只接受一个参数并将其打印到控制台，自动处理类型转换。  
无返回值。

### `input()` 函数

`input()` 函数用于读取用户输入，返回字符串类型数据：

```JavaScript
var x = input("请输入一些文本");
```

如需其他类型输入，可使用显式类型转换。  

## 条件判断

if...else if...else 语句
使用 if....else if...else 语句来选择多个代码块之一来执行。

```Javascript
if (condition1)
{
    当条件 1 为 true 时执行的代码
}
else if (condition2)
{
    当条件 2 为 true 时执行的代码
}
else
{
  当条件 1 和 条件 2 都不为 true 时执行的代码
};
```

## 循环

### while 语句

whup 中 while 语句的一般形式：

```JavaScript
while (条件){
    执行语句
};
```

### for 语句

whup for 循环可以遍历任何可迭代对象，如一个列表或者一个字符串。

for循环的一般格式如下：

```JavaScript
for <variable> in <sequence>{
    执行语句
};
```

同时，循环中应当支持break 和 continue 语句。

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
