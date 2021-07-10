#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>

int main() {
    //测试pow
    // int S = pow(2, 3);
    // int **ip = (int **) malloc(pow(2, 50) * sizeof(int *));
    // if (ip) {
    //     printf("分配成功\n");
    // }
    // else {
    //     printf("分配失败\n");
    // }

    //测试fopen的路径格式

    char c;
    unsigned long address;
    int length;
    char *filename = "traces/long.trace";
    FILE *fp = fopen(filename, "r");
    if (fp) {
        fscanf(fp, " %c %lx,%d", &c, &address, &length);//文件每一行行首有一个空格
        printf("%c %lx,%d\n", c, address, length);
        printf("路径格式合法\n");
    }
    else {
        printf("路径格式非法\n");
    }
    fclose(fp);
    return 0;
}