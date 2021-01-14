#include<stdio.h>

int threefourths(int x);

int main()
{
    
    return 0;
}
//todo
int threefourths(int x)
{
    int mult4 = x << 2;
    int w = sizeof(int) << 3;
    int mask = x >> (w -1);
    int bias = mask & (mult4 - 1);
    return 3.0 / mult4;
}