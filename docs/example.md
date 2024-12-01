# 示例

在 /docs/example 目录下，有多个whup示例。在这里我们将对每个示例进行简要的说明。

## [阶乘](example/factorial.whup)

```Javascript
function factorial(a:number):number{
    if(a==1 || a==0){
        return 1;
    }else{
        return a * factorial(a-1);
    };
};

d = factorial(5);
print(d);
```

阶乘函数，递归实现。  
输出：120

## [斐波那契数列](example/fibonacci.whup)

```Javascript
function fibonacci(a:number):number{
    if(a==1 || a==2){
        return 1;
    }else{
        return fibonacci(a-1)+fibonacci(a-2);
    };
};

d = fibonacci(9);
print(d);
```

斐波那契数列，递归实现。  
输出：34

## [汉诺塔](example/hanoi.whup)

```Javascript
function Hanoi(n : number) {
    if(n==1){
        ans = ans + 1;
    }
    else {
        Hanoi(n-1);
        Hanoi(1);
        Hanoi(n-1);
    };
};

var ans=0;
Hanoi(6);
print(ans);
```

汉诺塔问题，递归实现。  
输出：63
