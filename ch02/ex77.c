#include<stdio.h>

int main()
{
    printf("K = %d, %d\n", 17, (1 << 4) + 1);
    printf("K = %d, %d\n", -7, 1 - (1 << 3));
    printf("K = %d, %d\n", 60, (1 << 6) - (1 << 2));
    printf("K = %d, %d\n", -112, (1 << 4) - (1 << 7));

    return 0;
}