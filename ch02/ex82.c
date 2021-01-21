#include<stdio.h>

/* 
    TODO
        -C 证明
        -D 反例或证明 
        -E 反例或证明
 */
#define INT32_MIN (-2147483647-1)
int main()
{
    int x;
    int y;
    unsigned ux;
    unsigned uy;

    x = INT32_MIN;
    y = 0;
    printf("%d %s\n", (x < y) == (-x > -y), "(x < y) == (-x > -y)");

    //无符号和补码运算满足分配律,交换律和结合律
    printf("%d %s\n", ((x + y) << 4) + y - x == 17 * y + 15 * x, 
        "((x + y) << 4) + y - x == 17 * y + 15 * x");
    
    //因为-x = ~x + 1 <=> -x + x = ~x + x + 1 = ~1 + 1 = 0
    //所以~x + 1 + ~y + 1 = -x - y = -(x + y) = ~(x + y) + 1
    x = 0x1;
    y = 0x11;
    printf("%d %s\n", ~x + ~y + 1 == ~(x + y), "~x + ~y + 1 == ~(x + y)");

    //无符号和补码位级等价性 U2B = T2B
    //所以可以看作x - y == -(y -x)
    //因为补码满足分配律,所以成立
    ux = (unsigned) x;
    uy = (unsigned) y;
    printf("%d %s\n", (ux - uy) == - (unsigned) (y - x), "(ux - uy) == - (unsigned) (y - x)");

    //当x < 0时,x >> 2向下舍入,所以右移后小于右移前
    //当x >= 0时,x >> 2向零舍入,所以右移后小于右移前
    x = -1;
    printf("%d %s\n", ((x >> 2) << 2) <= x, "((x >> 2) << 2) <= x");
    return 0;
}