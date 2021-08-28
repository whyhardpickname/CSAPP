#include "vec.h"

#define IDENT 0
#define OP +

/* 创建包含指定长度的数组的结构 */
vec_ptr new_vec(long len) {
    /* 分配结构空间 */
    vec_ptr result = (vec_ptr) malloc(sizeof(vec_rec));
    data_t *data = NULL;
    if (!result) {
        return 0;/* 无法为结构分配空间 */
    }
    result->len = len;
    /* 分配数组空间 */
    if (len > 0) {
        data = (data_t *) calloc(len, sizeof(data_t));
        if (!data) {
            free((void *) result);
            return 0;/* 无法为数组分配空间 */
        }
    }
    result->data = data;
    return result;
}

/* 获取指定位置的元素，保存在dest。如果元素存在，返回1.否则返回0 */
int get_vec_element(vec_ptr v, long index, data_t *dest) {
    if (index < 0 || index >= v->len)
        return 0;
    *dest = v->data[index];
    return 1;
}

/* 返回向量大小 */
long vec_length(vec_ptr v) {
    return v->len;
}

void combine1(vec_ptr v, data_t *dest) {
    long i;
    
    *dest = IDENT;
    for (i = 0; i < vec_length(v); i++) {
        data_t val;
        get_vec_element(v, i, &val);
        *dest = *dest OP val;
    }
}

void combine2(vec_ptr v, data_t *dest) {
    long i;
    
    *dest = IDENT;
    long length = vec_length(v);
    for (i = 0; i < length; i++) {
        data_t val;
        get_vec_element(v, i, &val);
        *dest = *dest OP val;
    }
}

data_t *get_vec_start(vec_ptr v) {
    return v->data;
}

void combine3(vec_ptr v, data_t *dest) {
    long i;
    
    *dest = IDENT;
    long length = vec_length(v);
    data_t *data = get_vec_start(v);
    for (i = 0; i < length; i++) {
        data_t val = data[i];
        *dest = *dest OP val;
    }
}

void combine4(vec_ptr v, data_t *dest) {
    long i;
    
    long length = vec_length(v);
    data_t *data = get_vec_start(v);
    data_t acc = IDENT;
    for (i = 0; i < length; i++) {
        data_t val = data[i];
        acc = acc OP val;
    }
    *dest = acc;
}