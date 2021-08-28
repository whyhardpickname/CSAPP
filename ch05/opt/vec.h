#include<stdlib.h>

typedef long data_t;// long可以替换为其它类型。例如int，double
typedef struct {
    long len;
    data_t *data;
} vec_rec, *vec_ptr;