#include"block.h"
extern std::vector<ThreeAddressCode>tacs;
extern int globl_circulation_id;
class Break
{
    public:
    Break(Environment*);
};
Break::Break(Environment*env)
{
    std::string endLabelName="Circulation_endlabel_"+std::to_string(globl_circulation_id);//需要提前知道“键”是什么
    //这是在label生成时创建的“变量名”，借用变量名去查找label翻译出来的名字
    //所以关键在于在label生成时，合理地创建“变量名”
    std::string endLabel=env->get_var(endLabelName);//得到out.cpp会出现地的label名
    tacs.push_back({"goto","","",endLabel});
}