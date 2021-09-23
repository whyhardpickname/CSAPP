#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    if (argc != 2) {
        printf("Usage: ./hex2dd 0xab1223ff\n");
        return 1;
    }

    if (strlen(argv[1]) - 2 > 8) {
        printf("error: hexdecimal's len out of range\n");
        return 1;
    }

    unsigned dd;

    if (sscanf(argv[1], "0x%x", &dd) != 1) {
        printf("error: illegal format\n");
    }

    for (int i = 0; i < 4; i++) {
        printf("%u", (dd >> (i + 3) * 8) & (0xff));
        (i < 3) ? printf(".") : '\n';
    }
    
    printf("\n");
    return 0;
}
