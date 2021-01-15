#include<stdio.h>

int threefourths(int x);

int main()
{
    
    return 0;
}

/*
    计算3/4*x不溢出,向零舍入.则需要先除4再乘3.
    除4等价于向右移两位.设x的最后二位值为l,x = f + l.
    那么f无论正负都向零舍入,所以只要考虑l的舍入.
    l如果为负,加上偏差2^k -1再除即可.
*/

int threefourths(int x)
{
    int f = x & ~0x3;
    int l = x & 0x3;

    int w = sizeof(int) << 3;
    int mask = x >> (w - 1);
    int bias = mask & (l - 1);
    int fd4m3 = f >> 2;
    fd4m3 += fd4m3 << 1;

    int ld4m3 = l >> 2;
    ld4m3 += ld4m3 << 1;
    return fd4m3 + ld4m3 + bias;
}