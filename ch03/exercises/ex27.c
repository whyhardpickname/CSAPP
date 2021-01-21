#include<stdio.h>

long fact_for(int n);
long fact_while(int n);
long fact_goto(int n);

int main()
{
    
    return 0;
}
/* 阶乘,使用for循环 */
long fact_for(int n)
{
    long result = 1;
    for (int i = 2; i <= n; i++)
    {
        result *= i;
    }
    return result;
}

/* 阶乘,fact_for的while版,使用while循环.*/
long fact_while(int n)
{
    long result = 1;
    int i = 1;
    while (i <= n)
    {
        result *= i;
        i++;
    }
    return result;
}

/* 阶乘,fact_for的goto版,使用guarded-do循环翻译方法. */
long fact_goto(int n)
{
    long result = 1;
    int i = 1;
    if (n <= 1)
    {
        goto done;
    }
loop:
    result *= i;
    i++;
    if (i <= n)
        goto loop;
done:
    return result;
}