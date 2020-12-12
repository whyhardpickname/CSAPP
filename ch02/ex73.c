#include<stdio.h>

int saturating_add(int x, int y);

int main()
{
    
    return 0;
}

int saturating_add(int x, int y)
{
    int TMin = 0x80000000;
    int TMax = 0x7fffffff;
    
    int sum = x + y;
    int wide_minus_1 = (1 << 5) - 1;
    int xsign = x >> wide_minus_1;
    int ysign = y >> wide_minus_1;
    int sumsign = sum >> wide_minus_1;
    
    int xxory = xsign ^ ysign;

    
    return x + y;
}