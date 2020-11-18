#include<stdio.h>

int any_odd_bit(unsigned x)
{
    return !(x & 0x55555555);
}

int main()
{
    unsigned x = 0b1;
    unsigned y = 0b10;
    printf("%d\n", any_odd_bit(x));
    printf("%d\n", any_odd_bit(y));
}