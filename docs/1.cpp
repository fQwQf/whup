#include <iostream>

int main(){
    auto x = (2 && "4");
    int z = 2;
    int y = x - z;
    std::cout << y << std::endl;
    return 0;
}