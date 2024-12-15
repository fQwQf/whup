#include "WHUPstream.h"

extern bool print_c1;
extern bool print_c2;
extern bool print_e;

WHUPstream::WHUPstream(): std::ostream(this){}

int WHUPstream::overflow(int c){
        out(c);
        return 0;
}

WHUPstream_compile1::WHUPstream_compile1(): WHUPstream(){}

void WHUPstream_compile1::out(char c) {
    if (print_c1) {
        std::cout << c;
    }
}

WHUPstream_compile2::WHUPstream_compile2(): WHUPstream(){}

void WHUPstream_compile2::out(char c) {
    if (print_c2) {
        std::cout << c;
    }
}

WHUPstream_execute::WHUPstream_execute(): WHUPstream(){}

void WHUPstream_execute::out(char c) {
    if (print_e) {
        std::cout << c;
    }
}