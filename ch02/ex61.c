#include <stdio.h>

int main()
{
    int x = 0;
    printf("%x\n", !~x);
    printf("%x\n", !x);
    printf("%x\n", !(~x & 0xff));
    printf("%x\n", !(x >> ((sizeof(int) - 1) << 3)));
    return 0;
}