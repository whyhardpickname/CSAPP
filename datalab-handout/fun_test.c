#include<stdio.h>
int floatFloat2Int(unsigned uf) {
    unsigned exponent_mask = ((1 << 8) + ~1 + 1);
    unsigned exponent = (uf >> 23) & exponent_mask;

    unsigned significant_mask = ((1 << 23) + ~1 + 1);
    unsigned significant = uf & significant_mask;
    
    unsigned bias = (1 << 7) + ~1 + 1;
    unsigned sign = !!(uf & (1 << 31));
    int E = 0;
    int result = 0;
    
    //特殊值,包括无穷大和NaN.阶码值全为1
    if (!(exponent ^ exponent_mask))
    {
        return 0x80000000u;
    }
    //E < bias,即至多0111 1110,则加1后第8位为0
    if (!((exponent + 1) & (1 << 7)))
    {
        return 0;
    }
    return uf;
}
int main()
{
    printf("result   expetcted\n");
    printf("%-8x %-8x\n", floatFloat2Int(0x0), 0x0);
    return 0;
}