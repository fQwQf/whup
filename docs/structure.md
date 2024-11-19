# 关键结构

## 符号表

定义一个符号表类，具有链表结构，有一个指向父符号表的指针。  
内部有一个private的unordered_map，用来存储变量名，类型。  

具有以下几个方法：  

- insert：插入一个变量名，默认类型null。  
- find：查找一个变量名，返回类型。  
- change_type：修改一个变量的类型。  

## 一个生成label的组件

定义一个Label类，具有以下方法：  

- get_label：生成一个label名。label名是唯一的。
