#include<stdio.h>
int mult3div4(int x);
int main()
{
    printf("%d\n", mult3div4(-3));
    return 0;
}

int mult3div4(int x)
{
    int mult3 = (x << 1) + x;
    int w = sizeof(int) << 3;
    int mask = x >> (w - 1);
    int bias = mask & ((1 << 2) - 1);
    return (mult3 + bias) >> 2;
}
