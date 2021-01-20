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

    x = INT32_MIN;
    y = 0;
    printf("%d %s\n", (x < y) == (-x > -y), "(x < y) == (-x > -y)");

    
    printf("%d %s\n", ((x + y) << 4) + y - x == 17 * y + 15 * x, 
        "((x + y) << 4) + y - x == 17 * y + 15 * x");
    
    x = 0x1;
    y = 0x11;
    printf("%d %s\n", ~x + ~y + 1 == ~(x + y), "~x + ~y + 1 == ~(x + y)");
    return 0;
}