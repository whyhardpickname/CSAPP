#include<stdio.h>
int floatFloat2Int(unsigned uf) {
    int exponent_mask = ((1 << 8) + ~1 + 1);
    int exponent = (uf >> 23) & exponent_mask;

    int significant_mask = ((1 << 23) + ~1 + 1);
    int significant = uf & significant_mask;
    
    int bias = (1 << 7) + ~1 + 1;
    int sign = !!(uf & (1 << 31));
    int E = exponent - bias;
    int offset = 0;
    //尾数最高位前补1
    significant |= (1 << 23);
    //E < 0
    if (!(E ^ (1 << 31)))
    {
        return 0;
    }
    // E < 24,右移23 - E
    offset = (24 + ~E + 1);
    if (23 ^ (1 << 31))
    {
        significant <<= offset;
        if (sign)
        {
            return ~significant + 1;
        }
        return significant;
    }
    //E < 31,左移E - 23
    offset = (E + ~23 + 1);
    if (offset ^ (1 << 31))
    {
        significant >>= offset;
        if (sign)
        {
            significant >>= offset;
        }
        return significant;
    }
    return 0x80000000u;
}
int main()
{
    printf("result   expetcted\n");
    printf("%-8x %-8x\n", floatFloat2Int(0x0), 0x0);
    printf("%-8x %-8x\n", floatFloat2Int(0x3f800000), 0x1);
    printf("%-8x %-8x\n", floatFloat2Int(0xbf800000), 0xffffffff);
    printf("%-8x %-8x\n", floatFloat2Int(0x7f000000), 0x80000000);
    printf("%-8x %-8x\n", floatFloat2Int(0x3f800001), 0x1);
    return 0;
}