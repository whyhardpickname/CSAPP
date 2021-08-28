/*
 * mm-explicit.c -  memory-efficient malloc package.
 
 * 显式单向空闲链表 
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "mm.h"
#include "memlib.h"

/* If you want debugging output, use the following macro.  When you hand
 * in, remove the #define DEBUG line. */
#define DEBUG
#ifdef DEBUG
# define dbg_printf(...) printf(__VA_ARGS__)
#else
# define dbg_printf(...)
#endif


/* do not change the following! */
#ifdef DRIVER
/* create aliases for driver tests */
#define malloc mm_malloc
#define free mm_free
#define realloc mm_realloc
#define calloc mm_calloc
#endif /* def DRIVER */

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define SIZE_PTR(p)  ((size_t*)(((char*)(p)) - SIZE_T_SIZE))

#define NEXT_FITx

/* struct */
typedef long Align;//按long类型对齐
union header
{
    struct {
        union header *next;//指向下一个空闲块头部
        unsigned size;//块大小， 1 size= sizeof(long) = 8
    } s;
    Align x;
};

typedef union header Header;

/* $begin mallocmacros */
/* Basic constants and macros */
#define SEGSIZE 11 /* size class size */
#define CHUNKSIZE 256 /* Extend heap by this amount (sizeof(Header)) */  //line:vm:mm:endconst 
#define MAX(x, y) ((x) > (y)? (x) : (y))  

/* Global variables */
static Header *freep = NULL; /* 指向空闲块的指针 */ 
static Header *seg_list[SEGSIZE];

/* Function prototypes for internal helper routines */
static Header *extend_heap(unsigned words);
static void printblock(void *bp); 
static void checkheap(int verbose);
static void checkblock(void *bp);

/*  
 * mm_init - Called when a new trace starts.
 */
int mm_init(void) {
    for (int i = 0; i < SEGSIZE; i++) {
        if ((freep = (Header *) mem_sbrk(sizeof(Header))) == (void *)-1) { //line:vm:mm:begininit
            return -1;
        }
        freep->s.next = freep;
        freep->s.size = 0;
        seg_list[i] = freep;
    }
    return 0;
}

/*
 * malloc - Allocate a block by incrementing the brk pointer.
 *      Always allocate a block whose size is a multiple of the alignment.
 */
void *malloc(size_t nbytes) {
    
    size_t nuints, seg_units;      /* 每一unit为Align字节 */
    Header *p, *prevp, **pp, *tmp;
    /* Adjust block size to include overhead and alignment reqs. */
    nuints = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1; //line:vm:mm:sizeadjust3
    /* Search the free list for a fit */
    if (*seg_list == NULL) {
        mm_init();
    }
    //遍历查找nunits的size class
    seg_units = 2;
    for (int i = 0; i < SEGSIZE; i++, seg_units <<= 1) {
        if (nuints >= seg_units) {
            pp = seg_list + i;
        }
        else {
            break;
        }
    }
    //遍历各个链表
    for ( ; pp < seg_list + SEGSIZE; pp++) {
        prevp = *pp;
        for (p = prevp->s.next; ; prevp = p, p = p->s.next) {
            if (p->s.size >= nuints) {
                if (p->s.size - nuints < 2) {//最小的块为2,单位sizeof(Header)
                    prevp->s.next = p->s.next;
                }
                else {
                    p->s.size -= nuints;
                    tmp = p;
                    p += p->s.size;
                    p->s.size = nuints;
                    if (tmp->s.size < seg_units / 2) {
                        prevp->s.next = tmp->s.next;
                        free((void *) (tmp + 1));
                    }
                }
                printf("malloc(%p)\n", p);
                return (void *) (p + 1);
            }
            /* No fit found. Get more memory and place the block */
            if (p == *pp) {
                if (pp == seg_list + SEGSIZE - 1) {
                    if ((p = extend_heap(nuints)) == NULL) {
                        return NULL;
                    }
                }
                else {
                    break;
                }
            }
        }
    }
    return NULL;
}

/*
 * free - We don't know how to free a block.  So we ignore this call.
 *      Computers have big memories; surely it won't be a problem.
 */
void free(void *ap) {
    
    size_t seg_units;
    Header *p, *bp;
    bp = (Header *) ap - 1;
    if (ap == NULL) {
        return;
    }
    printf("free(%p)\n", bp);
    seg_units = 2;
    for (int i = 0; i < SEGSIZE; i++, seg_units <<= 1) {
        if (bp->s.size >= seg_units) {
            p = seg_list[i];
        }
        else {
            break;
        }
    }
    

    for ( ; !(p < bp && bp < p->s.next); p = p->s.next) {
        if (p >= p->s.next && (bp > p || bp < p->s.next)) {
            break;
        }
    }
    if (bp + bp->s.size == p->s.next) {
        bp->s.size += p->s.next->s.size;
        bp->s.next = p->s.next->s.next;
    }
    else {
        bp->s.next = p->s.next;
    }

    if (p + p->s.size == bp) {
        p->s.size += bp->s.size;
        p->s.next = bp->s.next;
    }
    else {
        p->s.next = bp;
    }

    freep = p;
}

/* 
 * extend_heap - Extend heap with free block and return its block pointer
 */
static Header *extend_heap(unsigned nuints) {
    printf("extend heap\n");
    char *p;
    Header *bp;

    nuints = MAX(nuints, CHUNKSIZE);
    /* Allocate an even number of words to maintain alignment */
    if ((long) (p = mem_sbrk(nuints * sizeof(Header))) == -1)  
            return NULL;                                        //line:vm:mm:endextend
    
    bp = (Header *) p;
    bp->s.size = nuints;
    free((void *) (bp + 1));
    return freep;                                          //line:vm:mm:returnblock
}


static void printblock(void *ap) 
{
    Header *bp;
    bp = (Header *) ap;
    printf("%p: header: [size: %d succeed: %p]\n", bp, bp->s.size, bp->s.next); 
}

static void checkblock(void *bp) 
{
    if ((size_t)bp % 8)
        printf("Error: %p is not doubleword aligned\n", bp);
}

/* 
 * checkheap - Minimal check of the heap for consistency 
 */
void checkheap(int verbose) 
{
    Header *p, *prevp;

    prevp = freep;
    for (p = prevp->s.next; ; prevp = p, p = p->s.next) {
        if (verbose) 
            printblock(p);
        checkblock(p);

        if (p == freep) {
            return;
        }
    }
}

/*
 * realloc - Change the size of the block by mallocing a new block,
 *      copying its data, and freeing the old block.  I'm too lazy
 *      to do better.
 */
void *realloc(void *oldptr, size_t size) {
  size_t oldsize;
  void *newptr;

  /* If size == 0 then this is just free, and we return NULL. */
  if(size == 0) {
    free(oldptr);
    return 0;
  }

  /* If oldptr is NULL, then this is just malloc. */
  if(oldptr == NULL) {
    return malloc(size);
  }

  newptr = malloc(size);

  /* If realloc() fails the original block is left untouched  */
  if(!newptr) {
    return 0;
  }

  /* Copy the old data. */
  oldsize = (((Header *) oldptr - 1)->s.size - 1) * sizeof(Header);
//   oldsize = *SIZE_PTR(oldptr);
  if(size < oldsize) oldsize = size;
  memcpy(newptr, oldptr, oldsize);

  /* Free the old block. */
  free(oldptr);

  return newptr;
}

/*
 * calloc - Allocate the block and set it to zero.
 */
void *calloc (size_t nmemb, size_t size)
{
  size_t bytes = nmemb * size;
  void *newptr;

  newptr = malloc(bytes);
  memset(newptr, 0, bytes);

  return newptr;
}

/*
 * mm_checkheap - There are no bugs in my code, so I don't need to check,
 *      so nah!
 */
void mm_checkheap(int verbose){
	  checkheap(verbose);
}
