#include <stdio.h>

int main()
{
    int x = 0x89ABCDEF;
    int y = 0x76543210;
    int bit_mask = 0xff;
    printf("%X", (x & bit_mask) | (y & ~bit_mask));
    return 0;
}