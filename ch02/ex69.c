#include<stdio.h>

unsigned rotate_left(unsigned x, int n)
{
    int w = sizeof(int) * 8;
    return (x >> (w - n)) | (x << n);
}

int main(int args, char *argv[])
{
    printf("0x%x\n", rotate_left(0x12345678, 0));
    printf("0x%x\n", rotate_left(0x12345678, 4));
    printf("0x%x\n", rotate_left(0x12345678, 20));
    return 0;
}