#include<stdio.h>

long test(long x, long y, long z);

int main()
{
    
    return 0;
}

long test(long x, long y, long z)
{
    long val = x + z + z;
    if (x < -3){
        if (y < z)
            val = x * y;
        else
            val = y * z;
    } else if (x > 2)
        val = x * z;
    return val;
}