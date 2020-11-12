#include<stdio.h>

unsigned srl(unsigned x, int k)
{
    unsigned xsra = (int) x >> k;
    unsigned w = sizeof(int) << 3;
    unsigned bit_mask = ~((~0) << (w - k));
    return xsra & bit_mask;
}

unsigned sra(int x, int k)
{
    int xsrl = (unsigned) x >> k;
    unsigned w = sizeof(int) << 3;
    unsigned bit_mask = xsrl - (xsrl & (1 << (w - k - 1)));
    return xsrl | bit_mask;
}

int main()
{
    unsigned x = 0xf2345678;
    printf("%x\n", srl(x, 4));
    printf("%x\n", sra(x, 4));
}