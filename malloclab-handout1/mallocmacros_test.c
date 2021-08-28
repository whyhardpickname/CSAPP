#include <stdio.h>

/* $begin mallocmacros */
/* Basic constants and macros */
#define WSIZE 4              /* Word and header/footer size (bytes) */ //line:vm:mm:beginconst
#define DSIZE 8              /* Double word size (bytes) */
#define CHUNKSIZE (1 << 12)    /* Extend heap by this amount (bytes) */  //line:vm:mm:endconst 

#define MAX(x, y) ((x) > (y) ? (x) : (y)) 

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))   //line:vm:mm:pack

/* Read and write a word at address p */
#define GET(P) ((unsigned) (*(p)))                 //line:vm:mm:get
#define PUT(P, val) (unsigned) (*(p) = (val))     //line:vm:mm:put

/* Read the size and allocated fields from address p */
#define GET_SIZE(p) (GET(P) & (~0X7))             //line:vm:mm:getsize
#define GET_ALLOC(p) (GET(P) & 0x1)             //line:vm:mm:getalloc

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp) ((char *) bp - WSIZE)                             //line:vm:mm:hdrp
#define FTRP(bp) ((char *) bp + GET_SIZE(HDRP(bp)) - DSIZE)        //line:vm:mm:ftrp

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT(bp) ((char *) bp + GET_SIZE((char *) bp - WSIZE))  //line:vm:mm:nextblkp
#define PREV(bp) ((char *) bp - GET_SIZE((char *) bp - DSIZE))  //line:vm:mm:prevblkp
/* $end mallocmacros */

void f();

int main() {
    printf("WSIZE=%d\n", WSIZE);
    printf("DSIZE=%d\n", DSIZE);
    printf("CHUNKSIZE=%d\n", CHUNKSIZE);
    printf("MAX(1, 2)=%d\n", MAX(1, 2));
    printf("PACK(8, 1)=%d\n", PACK(8, 1));
    int word;
    int *p = &word;
    PUT(p, PACK(8, 1));
    printf("GET(p)=%d\n", GET(p));
    printf("%d\n", GET_SIZE(p));
    printf("%d\n", GET_ALLOC(p));

    char chars[48];
    PUT(chars[0], PACK(16, 1));
    PUT(chars[12], PACK(16, 1));

    PUT(chars[16], PACK(16, 1));
    PUT(chars[28], PACK(16, 1));

    PUT(chars[32], PACK(16, 1));
    PUT(chars[42], PACK(16, 1));

    char *bp = &chars[20];
    printf("&chars[16] = %p\n", &chars[16]);
    printf("HDRP(bp) = %p\n", HDRP(bp));
    printf("&chars[4] = %p\n", &chars[4]);
    printf("PREV(bp) = %p\n", PREV(bp));
    

    printf("&chars[28] = %p\n", &chars[28]);
    printf("FTRP(bp) = %p\n", FTRP(bp));
    printf("&chars[36] = %p\n", &chars[36]);
    printf("NEXT(bp) = %p\n", NEXT(bp));

    printf("sizeof(size_t)=%lu\n", sizeof(size_t));
    printf("sizeof(int)=%lu\n", sizeof(int));
    
    printf("sizeof(size_t)=%lu\n", sizeof(size_t));
    printf("sizeof(unsigned)=%lu\n", sizeof(unsigned));
}