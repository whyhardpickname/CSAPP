#include<stdio.h>
#include"memlib copy.h"

/* 定义字，双字和块大小 */
#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE (1 << 12) /* 块每次增长的大小 */

#define MAX(x, y) ((x) > (y) ? (x) : (y)) 

/* 将块的大小和分配信息打包成一个字 */
#define PACK(size, alloc) ((size) | (alloc))

/* 从地址p中读写一个字 */
#define GET(p) (*(unsigned *)(p))
#define PUT(p, val) (*(unsigned *)(p) = (val))

/* 从地址p中读取块的大小和分配状态 */
#define GET_SIZE(p) (GET(p) & (~0x7))
#define GET_ALLOC(p) (GET(p) & (0x1))

/* 给定块指针，计算头部和脚部指针 */
#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* 给定块指针，计算前驱和后继块指针 */
#define PREP(bp) ((char *)(bp) - GET_SIZE(HDRP((bp) - DSIZE)))
#define SUCP(bp) ((char *)(bp) + GET_SIZE(HDRP((bp) - WSIZE)))

/* 全局变量 */
static char *heap_listp = 0; /* 指向序言块 */


/* 函数声明 */
static void *extend_heap(size_t words);
static void *coalesce(void *);
static void *find_fit(size_t asize);
static void place(void *bp, size_t asize);
int mm_init(void) {
    /* 创建一个空隐式链表 */
    if ((heap_listp = mem_sbrk(4 * WSIZE)) == (void *) -1) {
        return -1;
    }

    PUT(heap_listp, 0);/* 填充字 */
    PUT(heap_listp + WSIZE, PACK(DSIZE, 1));/* 序言头部字 */
    PUT(heap_listp + DSIZE, PACK(DSIZE, 1));/* 序言脚部字 */
    PUT(heap_listp + WSIZE + DSIZE, PACK(0, 1));/* 结尾字 */
    heap_listp += DSIZE;

    if (extend_heap(CHUNKSIZE / WSIZE) == NULL) {
        return -1;
    }
    return 0;
}

static void *extend_heap(size_t words) {
    size_t size;
    char *bp;

    /* 分配偶数字保持双字对齐 */
    size = (words % 2 == 0) ? words * WSIZE : (words + 1) * WSIZE;
    if ((long) (bp = mem_sbrk(size)) == -1) {
        return NULL;
    }
    PUT(HDRP(bp), PACK(size, 0));/* 普通头部字 */
    PUT(FTRP(bp), PACK(size, 0));/* 普通脚部字 */
    PUT(HDRP(SUCP(bp)), PACK(0, 1));/* 结尾字 */
    return coalesce(bp);    
}

void mm_free(void *bp) {
    size_t size = GET_SIZE(bp);

    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    coalesce(bp);
}

static void *coalesce(void *bp) {
    size_t prev_alloc = GET_ALLOC(FTRP(PREP(bp)));
    size_t succ_alloc = GET_ALLOC(HDRP(SUCP(bp)));
    size_t size = GET_SIZE(bp);

    if (prev_alloc && succ_alloc) {
        return bp;
    } 
    
    else if (!prev_alloc && succ_alloc){
        size += GET_SIZE(FTRP(PREP(bp)));
        PUT(HDRP(PREP(bp)), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
        bp = PREP(bp);
    } 
    
    else if (prev_alloc && !succ_alloc){
        size += GET_SIZE(FTRP(PREP(bp)));
        PUT(HDRP(bp), PACK(size, 0));/* FTRP使用头部字的信息 */
        PUT(FTRP(bp), PACK(size, 0));   
    } 
    
    else if (!prev_alloc && !succ_alloc){
        size += GET_SIZE(FTRP(PREP(bp)))
            + GET_SIZE(FTRP(PREP(bp)));
        PUT(HDRP(PREP(bp)), PACK(size, 0));
        PUT(FTRP(SUCP(bp)), PACK(size, 0));   
        bp = PREP(bp);
    }
    return bp;
}

void *mm_malloc(size_t size) {
    size_t asize;
    size_t extended_size;
    char *bp;

    if (heap_listp == 0) {
        mm_init();
    }

    if (size == 0) {
        return NULL;
    }

    if (size <= DSIZE) {
        asize = 2 * DSIZE;
    } 
    else {
        asize = DSIZE * ((size + DSIZE + DSIZE - 1) / DSIZE);
    }

    if ((bp = find_fit(asize)) != NULL) {
        place(bp, asize);
        return bp;
    }
    
    extended_size = MAX(asize, CHUNKSIZE);
    if ((bp = extend_heap((extended_size))) == NULL) {
        return NULL;
    }
    place(bp, extended_size);
    return bp;
}

/* 首次适配搜索 */
static void *find_fit(size_t asize) {
    char *bp;
    for (bp = heap_listp; GET_SIZE(bp) != 0; bp = SUCP(bp)) {
        if (!GET_ALLOC(HDRP(bp)) &&  (asize <= GET_SIZE(HDRP(bp)))) {
            return bp;
        }
    }   
    return NULL;
}

/* 放置头部和脚部字块 */
static void place(void *bp, size_t asize) {
    size_t csize = GET_SIZE(HDRP(bp));
    if (GET_SIZE(bp) - asize >= (2 * DSIZE)) {
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        bp = SUCP(bp);
        PUT(HDRP(bp), PACK(csize - asize, 0));
        PUT(FTRP(bp), PACK(csize - asize, 0));
    }
    else {
        PUT(HDRP(bp), PACK(csize, 1));
        PUT(FTRP(bp), PACK(csize, 1));
    }
}
