#include<stdio.h>

int saturating_add(int x, int y);

int main()
{
    printf("%x\n", saturating_add(0x80000000, 0x80000000));
    printf("%x\n", saturating_add(0x7fffffff, 0x7fffffff));
    return 0;
}

int saturating_add(int x, int y)
{
    int TMin = 0x80000000;
    int TMax = 0x7fffffff;

    int wide_minus_1 = (1 << 5) - 1;

    int sum = x + y;
    int x_sign = x >> wide_minus_1;
    int y_sign = y >> wide_minus_1;
    int sum_sign = sum >> wide_minus_1;

    int is_neg_overflow = x_sign & y_sign & ~sum_sign;
    int is_pos_overflow = ~x_sign & ~y_sign & sum_sign;
    int overflow = is_neg_overflow | is_pos_overflow;
    return (is_neg_overflow & TMin) | (is_pos_overflow & TMax) | (~overflow & sum);
}