#include <stdio.h>

int fits_bits(int x, int n)
{
    //20201125 todo
    int w = sizeof(int) << 3;
    int mask = (2 << (n - 1));
    int rshift = (x >> (w - n)) & mask;
    return x == (rshift | ((mask - 1) & x));
}
int main()
{
    printf("%d\n", fits_bits(-1, 2));
    return 0;
}

