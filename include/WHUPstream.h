#include <bits/stdc++.h>

struct WHUPstream : std::ostream , std::streambuf{
    WHUPstream(): std::ostream(this){}
    int overflow(int c){
        out(c);
        return 0;
    }

    virtual void out(char c) = 0;
};

struct WHUPstream_compile1 : WHUPstream{
    WHUPstream_compile1(): WHUPstream(){}
    void out(char c);
};

struct WHUPstream_compile2 : WHUPstream{
    WHUPstream_compile2(): WHUPstream(){}
    void out(char c);
};

struct WHUPstream_execute : WHUPstream{
    WHUPstream_execute(): WHUPstream(){}
    void out(char c);
};