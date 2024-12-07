# tac

以下记录了所有可能用到的tac。参数是以op,arg1,arg2,result的形式排列的。

## =

| op | arg1 | arg2 | result |
|---|---|---|---|
| = | arg1 |  | result |

效果：将arg1的值赋给result。

## 运算符

| op | arg1 | arg2 | result |
|---|---|---|---|
| + | arg1 | arg2 | result |
| - | arg1 | arg2 | result |
| * | arg1 | arg2 | result |
|......|......|......|......|

效果：arg1和arg2进行运算，将结果赋给result。

## label

| op | arg1 | arg2 | result |
|---|---|---|---|
| label | |  | name |

效果：将name作为label，以供跳转。

## if_goto

| op | arg1 | arg2 | result |
|---|---|---|---|
| if_goto | arg1 |  | label |

效果：如果arg1为真，则跳转到label。

## goto

| op | arg1 | arg2 | result |
|---|---|---|---|
| goto | |  | label |

效果：跳转到label。

## print

| op | arg1 | arg2 | result |
|---|---|---|---|
| print | arg1 |  |  |

效果：将arg1的值输出到控制台。

## input

| op | arg1 | arg2 | result |
|---|---|---|---|
| input | |  | result |

效果：从控制台输入一个值，赋给result。

## push

| op | arg1 | arg2 | result |
|---|---|---|---|
| push | arg1 |  |  |

效果：将arg1的值压入栈中。

## pop

| op | arg1 | arg2 | result |
|---|---|---|---|
| pop | |  | result |

效果：将栈顶的值弹出，赋给result。

## call

| op | arg1 | arg2 | result |
|---|---|---|---|
| call | label |  |  |

效果：跳转到label，并且将当前所在的位置压入返回栈中。

## return

| op | arg1 | arg2 | result |
|---|---|---|---|
| return | |  |  |

效果：从返回栈中弹出位置，跳转到该位置。

## ptr

| op | arg1 | arg2 | result |
|---|---|---|---|
| ptr | arg1 |  | result |

效果：将arg1的指针指向result。  

## load

| op | arg1 | arg2 | result |
|---|---|---|---|
| load | arg1 |  | result |

效果：将arg1的值赋给result。  

以下是数组相关的操作：

## new_array

| op | arg1 | arg2 | result |
|---|---|---|---|
| new_array | arg1 |    | result |

效果：创建一个长度为arg1的数组，并将指针赋给result。  
注意：WHUPvm的数组本质上是在变量栈内存中预留一段，而变量栈内存的都是void指针，在理论上来说，数组的元素可以是任何类型。但实际上在whu的语法中是否允许，请自行决定。  
另外，多维数组也请转化为一维数组。  
至于嵌套数组，只需要将数组指针作为父数组的元素即可。  

## array_set

| op | arg1 | arg2 | result |
|---|---|---|---|
| array_set | arg1 | arg2 | result |

效果：将arg2的值赋给result数组中索引为arg1的元素。  
注意：本质上是赋给result指针加上偏移量arg1的地址的元素。  

## array_get

| op | arg1 | arg2 | result |
|---|---|---|---|
| array_get | arg1 | arg2 | result |

效果：将arg1的数组中索引为arg2的元素赋给result。  

## array_size

| op | arg1 |arg2 | result |
| --- | --- | --- | --- |
| array_size | arg1 |   | result |

效果：将arg1的数组的长度赋给result。  

以下是