#include<stdio.h>
int howManyBits(int x) {
  int temp = x;
  x ^= (x << 1);
  x |= (x >> 1);
  x |= (x >> 2);
  x |= (x >> 4);
  x |= (x >> 8);
  x |= (x >> 16);
  int count = 0;
  
  temp = !temp;
  temp = ~temp + 1;
  return (~temp & count) | (temp & 1);
}

int main()
{
    printf("non-negative: \n");
    printf("5\t%d\n", howManyBits(12));
    printf("10\t%d\n", howManyBits(298));
    printf("1\t%d\n", howManyBits(0));
    printf("negative: \n");
    printf("4\t%d\n", howManyBits(-5));
    printf("1\t%d\n", howManyBits(-1));
    printf("32\t%d\n", howManyBits(0x80000000));
    return 0;
}