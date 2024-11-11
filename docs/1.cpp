#include <iostream>

int main()
{
    int a = 4;
    bool t1 = a > 3;
    bool t2 = a == 3;
    bool t3 = a < 3;

    if (t1)
        goto l1;
    if (t2)
        goto l2;
    if (t3)
        goto l3;
    std::cout << 4;
    goto end;

l1:
    std::cout << 1;
    goto end;

l2:
    std::cout << 2;
    goto end;

l3:
    std::cout << 3;
    goto end;

end:
    return 0;
}   