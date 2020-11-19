#include<stdio.h>

/*
int bad_int_size_is_32()
{
    
    int set_msb = 1 << 31;

    int beyond_msb = 1 << 32;   //移位数大于机器字长

    return set_msb && !beyond_msb;
}
*/
int int_size_is_32()
{
    
    int set_msb = 1 << 31;
    int beyond_msb = set_msb << 1;
    return set_msb && !beyond_msb;
}

int int_size_is_16_for_16bits()
{
    int set_msb = 1 << 15 << 15 << 1;
    int beyond_msb = set_msb << 1;
    return set_msb && !beyond_msb;
}

int main(int args, char *argv[])
{
    printf("%d\n", int_size_is_32());
    printf("%d", int_size_is_16_for_16bits());
    return 0;
}