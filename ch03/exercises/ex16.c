#include<stdio.h>

void cond(long a, long *p);

int main()
{
    
    return 0;
}
/*
    goto版cond
*/
void cond(long a, long *p)
{
    if (!p)
    {
        goto L1;
    }
    if (a <= *p)
    {
        goto L1;
    }
    *p = a;
L1:
    return;
}
