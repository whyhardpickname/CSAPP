#include<stdio.h>

int divide_power2(int x, int k);

int main()
{
    
    return 0;
}

int divide_power2(int x, int k)
{
    int w = sizeof(int) * 8;
    int mask = x >> (w - 1);
    int bias = mask & ((1 << k)  - 1);
    return (x + bias) >> k;
}