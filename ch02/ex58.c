#include <stdio.h>

typedef unsigned char *byte_pointer; 
int is_little_endian()
{
    int val = 1;
    byte_pointer start = (byte_pointer) &val;
    return *start == val;
}

int main()
{
    printf("%d", is_little_endian());
    return 0;
}
  