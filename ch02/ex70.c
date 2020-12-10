#include<stdio.h>

int fit_bits(int x, int n);

int main()
{
    
    printf("%10s %2s %s %s\n", "input", "input", "output", "expetcted");
    printf("%10x %2d %d %d\n", 0xffffffff, 1, fit_bits(0xffffffff, 1), 1);
    printf("%10x %2d %d %d\n", 0xffffffff, 2, fit_bits(0xffffffff, 2), 1);
    printf("%10x %2d %d %d\n", 0x00000001, 1, fit_bits(0x00000001, 1), 0);
    printf("%10x %2d %d %d\n", 0x00000001, 2, fit_bits(0x00000001, 2), 1);
    printf("%10x %2d %d %d\n", 0x00000001, 3, fit_bits(0x00000001, 3), 1);
    printf("%10x %2d %d %d\n", 0x7fffffff, 31, fit_bits(0x7fffffff, 31), 0);
    printf("%10x %2d %d %d\n", 0x7fffffff, 32, fit_bits(0x7fffffff, 32), 1);
    printf("%10x %2d %d %d\n", 0xfffff00f, 13, fit_bits(0xfffff00f, 13), 1);
    printf("%10x %2d %d %d\n", 0xfffff00f, 12, fit_bits(0xfffff00f, 12), 0);
    printf("%10x %2d %d %d\n", 0xffff000f, 17, fit_bits(0xffff000f, 17), 1);
    printf("%10x %2d %d %d\n", 0xffff000f, 16, fit_bits(0xffff000f, 16), 0);


    return 0;
}

int fit_bits(int x, int n)
{
    int count = (sizeof(int) << 3) - n;
    int leftright = (x << count) >> count;
    return x == leftright;
}