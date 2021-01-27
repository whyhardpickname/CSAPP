#include<stdio.h>

int bitCount2bit(int x);
int bitCount4bit(int x);
int bitCount8bit(int x);

int main()
{
    
    return 0;
}

int bitCount2bit(int x)
{
    int mask = 0b01;
    return (x & mask) + (x >> 1) & mask;
}
int bitCount4bit(int x)
{
    int mask = 0b0101;
    int halfSum = x & mask + (x >> 1) & mask;

    int mask2 = 0b0011;
    return halfSum & mask2 + (halfSum >> 2) & mask2;
}
int bitCount8bit(int x)
{
    int mask = 0b01010101;
    int quarterSum = x & mask + (x >> 1) & mask;

    int mask2 = 0b00110011;
    int halfSum = quarterSum & mask2  + (quarterSum >> 2) & mask2;

    int mask3 = 0b00001111;
    return halfSum & mask3 + (halfSum >> 4) & mask3;
}