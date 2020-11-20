#include<stdio.h>
int lower_one_mask(int n)
{
    return (2 << (n - 1)) - 1;
}
int main(int args, char *argv[])
{
    printf("0x%x\n", lower_one_mask(6));
    printf("0x%x\n", lower_one_mask(17));
    printf("0x%x\n", lower_one_mask(32));
    return 0;
}