#include<stdio.h>

int tsub_ok(int x, int y);

int main()
{
    printf("%x\n", tsub_ok(0x80000000, 0x70000000));
    printf("%x\n", tsub_ok(0x7fffffff, 0x8fffffff));
    return 0;
}

int tsub_ok(int x, int y)
{
    int wide_minus_1 = (1 << 5) - 1;

    int sub = x - y;
    int x_sign = x >> wide_minus_1;
    int y_sign = y >> wide_minus_1;
    int sub_sign = sub >> wide_minus_1;

    int is_neg_overflow = x_sign & ~y_sign & ~sub_sign;
    int is_pos_overflow = ~x_sign & y_sign & sub_sign;
    int overflow = !(is_neg_overflow | is_pos_overflow);
    return !overflow;   
}