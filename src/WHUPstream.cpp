#include "WHUPstream.h"

extern bool print_c1;
extern bool print_c2;
extern bool print_e;

void WHUPstream_compile1::out(char c) {
    if (print_c1) {
        std::cout << c;
    }
}

void WHUPstream_compile2::out(char c) {
    if (print_c2) {
        std::cout << c;
    }
}

void WHUPstream_execute::out(char c) {
    if (print_e) {
        std::cout << c;
    }
}