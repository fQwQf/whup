#include <iostream>

int main()
{
    std::clock_t start = clock();

    int j = 1;
    long long sum = 0;
    while (j <= 100)
    {
        int i = 3;
        
        while (i <= 3000)
        {
            int isprime = 1;
            int b = 2;
            while (b * b <= i)
            {
                if (i % b == 0)
                {
                    isprime = 0;
                };
                b = b + 1;
            };
            if (isprime == 1)
            {
                sum = sum + i;
            };
            i = i + 1;
        };
        j = j + 1;
    };
    std::cout << "sum = " << sum << std::endl;
    std::cout << "done" << std::endl;

    std::clock_t end = clock();
    std::cout << "Wall clock time:" << (double)(end - start) / CLOCKS_PER_SEC << "s" << std::endl;
}