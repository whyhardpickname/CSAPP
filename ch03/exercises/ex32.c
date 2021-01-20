#include<stdio.h>
#define INT32_MAX (2147483647)
#define INT64_MAX (9223372036854775807)

int fac(long n);

int main()
{
    printf("%d! <= INT32_MAX\n", fac(INT32_MAX));
    printf("%d! <= INT64_MAX\n", fac(INT64_MAX));
    return 0;
}

/* 
    返回n! <= max 时n的最大值
*/
int fac(long max)
{
    int i;

    i = 2;
    //使用乘法需要处理溢出,使用除法不用
    while ((max /= i) > 0)
    {
        i++;
    }
    
    return --i;
}