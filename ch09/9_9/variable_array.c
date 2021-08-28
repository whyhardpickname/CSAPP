#include<stdio.h>
#include<stdlib.h>

int main() {
    int n, i;
    printf("请输入要输入的数字数目: ");
    scanf("%d", &n);

    int *array = (int *) malloc(n * sizeof(int));
    if (!array) {
        printf("内存分配失败");
        return 1;
    }
    for (i = 0; i < n; i++) {
        array[i] = scanf("%d", &array[i]);
    }
    return 0;
}