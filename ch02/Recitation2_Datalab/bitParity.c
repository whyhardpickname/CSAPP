#include<stdio.h>

int bitParity2bit(int x);
int bitParity4bit(int x);
int bitParity8bit(int x);

int main()
{
    
    return 0;
}

/* 分而治之 */
/* 返回整数x位表示中1的个数的奇偶性,偶数个1返回0,奇数个1返回1 */

/* 位长为1时 */
int bitParity2bit(int x)
{
    int mask = 0b01;
    return (x & mask) ^ ((x >> 1) & mask);
}

/* 位长为4时 */
int bitParity4bit(int x)
{
    int mask = 0b0101;
    int halfSum = (x & mask) ^ ((x >> 1) & mask);

    int mask2 = 0b0011;
    return (halfSum & mask2) ^ ((halfSum >> 2) & mask2);
}

/* 位长为8时 */
int bitParity8bit(int x)
{
    int mask = 0b01010101;
    int quarterSum = (x & mask) ^ ((x >> 1) & mask);

    int mask2 = 0b00110011;
    int halfSum = (quarterSum & mask) ^ ((quarterSum >> 2) & mask);

    int mask3 = 0b00001111;
    return (halfSum & mask3) ^ ((halfSum >> 4) & mask3);
}

/* 总结
    设2^n = w,w为位长
    则第i阶段有
    mask_i = 0b(0^(2^(i-1))1^(2^(i-1)))^(2^(n-i));
    sum_i = sum_(i-1) & maks_i ^ ((sum_(i-1) >> i) & mask_i)
 */