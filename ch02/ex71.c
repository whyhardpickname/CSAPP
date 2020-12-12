#include<stdio.h>

typedef unsigned packed_t;

int xbyte(packed_t word, int bytenum);

int main()
{
    
    return 0;
}

int xbyte(packed_t word, int bytenum)
{
    return (word << ((3 - bytenum) << 3) >> 24);
}