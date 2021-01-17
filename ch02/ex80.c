#include<stdio.h>

int threefourths(int x);
int threefourths_v2(int x);

int main()
{
    printf("%d\n", threefourths(4));
    printf("%d\n", threefourths(-4));
    printf("%d\n", threefourths_v2(4));
    printf("%d\n", threefourths_v2(-4));

    return 0;
}

/*
    计算3/4*x不溢出,向零舍入.
    3/4*x = x/2 + x/4
    根据最后2位和1位,是否补1有三种情况
    一,00,补0
    二,01和10,补1
    三,11补2
*/

int threefourths(int x)
{
    int x2 = x & 0x3;
    int x1 = (x & 0x1) << 1;

    int x_mask = (sizeof(int) << 3) - 1;
    int bias = x_mask & 0x3;
    int incr = (x2 + x1 + bias) >> 2;

    int s2 = x >> 2;
    int s1 = x >> 1;

    return s1 + s2 + incr;
}

/* 
    计算3/4*x,不溢出,向零舍入
    先除后乘防止溢出,再令x = l + r;
    r是最右边2位比特.
    l无需考虑舍入.
 */
int threefourths_v2(int x)
{
    int l = x & ~0x3;
    int r = x & 0x3;

    int x_mask = (sizeof(int) << 3) - 1;
    int bias = x_mask & 0x3;
    int incr = (r + bias) >> 2;

    int s1 = l >> 2;
    s1 += (s1 << 1);

    incr += (incr << 1);

    return s1 + incr;
}