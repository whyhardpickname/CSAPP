#include<stdio.h>
//todo
/* 
    产生指定的位模式
    w表示数据类型的位数    
 */
int main()
{
    //1^(w-k)0^(k)
    int k = 5;
    int A = ~((1 << k) - 1);
    printf("%x\n", A);
    return 0;
}