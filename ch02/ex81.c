#include<stdio.h>
//todo
/* 
    产生指定的位模式
    w表示数据类型的位数    
 */
int main()
{
    //1^(w-k)0^(k) = 1^w - 1^k
    int k = 4;
    int A = ~((1 << k) - 1);
    printf("%x\n", A);
    
    //0^(w-k-j)1^(k)0^(j) = 0^(w-k-j)1^(k+j) - 1^j
    int j = 4;
    int B = (1 << (k + j)) - 1;
    B -= ((1 << j) - 1);
    printf("%x\n", B);
    return 0;
}