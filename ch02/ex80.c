#include<stdio.h>

int threefourths(int x);

int main()
{
    printf("%d\n", threefourths(4));
    printf("%d\n", threefourths(-4));

    return 0;
}

/*
    计算3/4*x不溢出,向零舍入.
    3/4*x = x/2 + x/4
    根据最后2位和1位,是否补1有三种情况
    一,00,补0
    二
*/

int threefourths(int x)
{
    int x2 = x & 0x3;
    int x1 = (x & 0x1) << 1;

    int x_mask = (sizeof(int) << 3) - 1;
    int bias = x_mask & 0x3;
    int incr
}