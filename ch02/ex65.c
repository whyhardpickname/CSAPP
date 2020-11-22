#include<stdio.h>

int odd_ones(unsigned x)
{
    x ^= (x >> 16);
    x ^= (x >> 8);
    x ^= (x >> 4);
    x ^= (x >> 2);
    x ^= (x >> 1);
    return x & 1;
}

int main(int args, char *argv[])
{
    printf("%d\n", odd_ones(0b111));
    printf("%d\n", odd_ones(0b1111));
    return 0;
}