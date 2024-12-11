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

## REFNUM

| op | arg1 | arg2 | result |
|---|---|---|---|
| REFNUM | arg1 |  | result |

效果：使result指向arg1

## BIASNUM

| op | arg1 | arg2 | result |
|---|---|---|---|
| BIASNUM | arg1 | arg2 | result |

效果：使result指向arg1偏移arg2的值个float单位长度。

## BIASSTR

| op | arg1 | arg2 | result |
|---|---|---|---|
| BIASSTR | arg1 | arg2 | result |

效果：使result指向arg1偏移arg2的值个string单位长度。
