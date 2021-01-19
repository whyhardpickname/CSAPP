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
        goto done;
    }
    if (*p >= a)
    {
        goto done;
    }
    *p = a;
done:
    return;
}
