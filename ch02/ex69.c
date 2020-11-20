#include<stdio.h>

unsigned rotate_left(unsigned x, int n)
{
    unsigned w = sizeof(int) * 8;
    int z_mask = -!n;
    unsigned mask = ((2 << (n - 1)) - 1) << (w - n);
    unsigned n_msb = (x & mask) >> (w - n);
    return (z_mask & x) | (~z_mask & (x << n) | n_msb);
}

int main(int args, char *argv[])
{
    printf("0x%x\n", rotate_left(0x12345678, 0));
    printf("0x%x\n", rotate_left(0x12345678, 4));
    printf("0x%x\n", rotate_left(0x12345678, 20));
    return 0;
}