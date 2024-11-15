#include "block.h"
#include <bits/stdc++.h>

/*
*构建block最主要的目的是便于确定变量作用域。
*每个block都要指向一个environment对象。
*每生成一个新的block，都要将当前block的environment对象作为参数传入新block的构造函数中。
*/
class Block{
    std::vector<ThreeAddressCode> code;
    std::vector<Block> children;
    Environment* env;
public:
    Block(Environment* e):env(e){}
    
};