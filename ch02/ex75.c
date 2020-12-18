#include<stdio.h>

unsigned unsigned_high_prod(unsigned x, unsigned y);
int signed_high_prod(int x, int y);

int main()
{
    
    return 0;
}

unsigned unsigned_high_prod(unsigned x, unsigned y)
{
    int xsign = x >> 31;
    int ysign = y >> 31;
    unsigned p = (unsigned) signed_high_prod((int) x, (int) y);
    return p + xsign & y + ysign & x;
}