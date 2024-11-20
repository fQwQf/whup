#include"break.h"
std::stack<int> global_circulation_id;
//用一个全局栈来记录循环的id，这样就可以在break语句中找到对应的循环的结束标签了。
Break::Break(Environment*env)
{
    std::string endLabelName="Circulation_endlabel_"+std::to_string(global_circulation_id.top());
    //需要提前知道“键”是什么
    //这是在label生成时创建的“变量名”，借用变量名去查找label翻译出来的名字
    //所以关键在于在label生成时，合理地创建“变量名”
    if(global_circulation_id.empty())
    {
        std::cout<<"break语句错误，没有对应的循环"<<std::endl;
        exit(0);
    }
    global_circulation_id.pop();
    std::string endLabel=env->get_var(endLabelName);//得到out.cpp会出现地的label名
    tacs.push_back({"goto","","",endLabel});
}