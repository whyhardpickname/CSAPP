#include<stdio.h>
long lt_cnt;
long ge_cnt;
long gotodiff_se(long x, long y);

int main()
{
    
    return 0;
}

/*
    基于以下格式的goto版c语言
        t = test-expr;
        if (t)
            goto true;
        else-statement;
        goto done;
    true:
        then-statement;
    done:
*/

long gotodiff_se(long x, long y)
{
    long result;
    if (x < y)
        goto L1;
    ge_cnt++;
    result = x -y;
    return result;
L1:
    lt_cnt++;
    result = y - x;
    return result;
}
