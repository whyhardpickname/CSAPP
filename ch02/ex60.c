#include <stdio.h>
unsigned replace_byte(unsigned x, int i, unsigned char b)
{
    int move = i * 8;
    return (x & ~(0xff << move)) | (b << move);

}

int main()
{
    printf("%X\n", replace_byte(0x12345678, 2, 0xAB));
    printf("%X\n", replace_byte(0x12345678, 0, 0xAB));
    return 0;
}