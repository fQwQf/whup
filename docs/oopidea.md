# 类的概述：



## 会用到的结构：

1.类表，<classname,Class*> 目前只储存了声明语句

2.实例表<objectname,Object*> 储存所有的实例

3.实例的函数表<classFunctionName,ClassFunction*> 每一个实例都拥有自己的函数表，在调用时根据实例名得到该函数表，在根据函数名在表中查找到函数

4.实例的符号表<string,string> 每一个实例都拥有自己的符号表，用来记录数据成员。
  在whup中该符号表是不可以被用户直接访问的，因为我们默认所有数据成员都是private的
  该符号表在成员函数中是可以访问的。


## 实现过程：

eg:
```c++
class Person
{
    var age;
    var name;
    Person(x:int,y:string)
    {
        name=x;
        age=y;
        print("create successfully!!!");
    };
    function f()
    {
        print(name);
    };
};

Person p1(18,"nihao");
p1->f();
```

1.声明时，由class关键字开启声明

2.Class类负责将声明语句插入类表

3.创建类的实例时，根据类名得到对应的声明语句

4.Object类负责处理声明语句：

  a.对于var变量，交给Var类处理，处理后会将变量名插入该实例的符号表

  b.对于function，交给classfunction类来处理，处理后会将函数名插入该实例的函数表

  c.对于构造函数，我们借鉴c++的定义方式
    构造函数不会被插入到实例的函数表，但会被直接记录Object中
    构造函数与其他函数成员函数的区别：
        调用方式：创建类时自动调用，对参数的处理方法相同

5.用->调用成员函函数


