#include<stdio.h>

int leftmost_one(unsigned x)
{
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >> 16);
    return x ^ (x >> 1);
}

int main(int args, char *argv[])
{
    printf("%x\n", leftmost_one(0x0000));
    printf("%x\n", leftmost_one(0xff00));
    printf("%x\n", leftmost_one(0x6600));
    return 0;
}