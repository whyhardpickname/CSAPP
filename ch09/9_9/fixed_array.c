#include<stdio.h>
#define MAXN 100    

int array[MAXN];

int main() {
    int n, i;
    scanf("%d", &n);
    if (n >= MAXN) {
        printf("The input numbes are too big.");
    }
    for (i = 0; i < n; i++) {
        scanf("%d", &array[i]);
    }
    return 0;
}
