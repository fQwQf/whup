#include "WHUPstream.h"

bool print_c1 = false;
bool print_c2 = false;
bool print_e = false;

WHUPstream_compile1 WHUPout;
WHUPstream_compile2 WHUPout_c2;
WHUPstream_execute WHUPout_e;
WHUPstream_number WHUPout_number;

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
