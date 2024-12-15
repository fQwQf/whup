#include <bits/stdc++.h>

#ifndef WHUPstream_H
#define WHUPstream_H

struct WHUPstream : std::ostream , std::streambuf{
    WHUPstream();
    int overflow(int c);
    virtual void out(char c) = 0;
};

struct WHUPstream_compile1 : WHUPstream{
    WHUPstream_compile1();
    void out(char c);
};

struct WHUPstream_compile2 : WHUPstream{
    WHUPstream_compile2();
    void out(char c);
};

struct WHUPstream_execute : WHUPstream{
    WHUPstream_execute();
    void out(char c);
};

#endif