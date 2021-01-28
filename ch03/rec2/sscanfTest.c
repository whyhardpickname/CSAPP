#include<stdio.h>

int main()
{
    char *input = "123.1, 456";
    double a;
    int b;
    sscanf(input, "%lf, %d", &a, &b);
    printf("%f\n", a);
    printf("%d\n", b);
    return 0;
}