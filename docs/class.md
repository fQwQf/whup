# 各种类的处理

我认为变量最好翻译为变量自身名_符号表id_类型，这样进行类型转换是就不必再删除然后新建。因为，在将所有声明都放在最前面的情况下，中途新建是不可行的。例如，在id为2的符号表中有：

```Javascript
var a = 2;
```

就会翻译为：

```c++
int a_2_int = 2;
```

## block

即语句块。  
对于每个block，我们会对其按照没有被(),{}包裹的;划分为stmt。  
每个block都对应一个符号表，用于存储变量。  

## stmt

对于一个stmt，我们先识别第一个token。如果为标识符，则为赋值语句。如果为关键字，则为对应的类。  

## assign

即为赋值类。会检查类型，并调用符号表的change_type方法。  
注意：若赋值号两边类型不同且左值类型不为null，则左值翻译出的c++应先delete再创建一个新的。  

## expression

对于每个expression，在创建时，先在符号表中查找所有为标识符类型的token的变量类型和symbol的种类。如果运算符出现比较或布尔运算，则认为结果是布尔；如果出现字符串，则认为结果为字符串；否则为数字。会由父节点创建对应类型的临时变量。  
接下来会按照运算符优先级扫描，并分解为多个expression。  
值得一提的是，对于+，如果两个对象为一个字符串和一个数字，应当对数字进行类型转换。  
对于类型的运算，若检查出非数字，则报错。  
注意expression是反序压入的。  

## var

直接调用该层符号表的相应方法，插入一个变量名。  

## if...else if ... else

示例如下：

```Javascript
if (a > 3){
    stmt1;
}else if(a == 3){
    stmt2;
}else if(a < 3){
    stmt3;
}else{
    stmt4;
};
```

```c++
bool t1 = a > 3;
bool t2 = a == 3;
bool t3 = a < 3;

if (t1) goto l1;
if (t2) goto l2;
if (t3) goto l3;
goto l4;

l1:
    stmt1;
    goto end;

l2:
    stmt2;
    goto end;

l3:
    stmt3;
    goto end;

l4:
    stmt4;
    goto end;
end:

```

## while

示例如下：

```Javascript
//a=1;

while(a<4){
    a++;
    if(a==3){
        continue;
    };
    a++;
};
```

```c++
bool t1;
bool t2;

l0:
    t1 = a < 4;
    if (t1) goto l1;
    goto end;

l1:
    a++ ;
    t2 = (a == 3) ;
    if (t2) goto l1;
    a++;
    goto l0;

end:
```

break直接goto end。  
注意：goto不能跳过变量声明，因此要在语句前声明所有临时变量。  

## function

实现函数调用。  
每个函数调用都会创建一个临时变量，用于存储返回值。  
同时还有几个临时变量，用于存储参数，即形参。  
当遇到调用函数的语句时，会先将实参赋给形参，然后跳转至函数体。  
函数return后，将return的值赋给临时变量，然后跳转至调用语句 。  
在调用语句中，函数体被替换成临时变量。  

**TODO**  
函数有时可能需要访问外部变量。可以像python一样规定一个关键字，被该关键字标明的变量同样会被视为形参，在调用时识别并传入；也可以像c++一样规定最大的Environment中的是全局的，所有函数的Environment指向它，因此可以访问全局变量。当然，完全禁止也可以。  

```Javascript
function add(a,b){
    return a+b;
}
var a=add(1,2);
```

生成的中间代码为：

```C++
int t1;//形参
int t2;//形参
int t3;//临时变量
int t4 = 1;//实参
int t5 = 2;//实参

l0:
    t1 = t4;
    t2 = t5;
    goto l1;
l2:
    a = t3;

//中间略

l1:
    t3 = t1 + t2;
    goto l2;


```
