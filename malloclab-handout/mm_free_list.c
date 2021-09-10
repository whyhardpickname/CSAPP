/*
 * mm-implicit.c -  memory-efficient malloc package.
 *
 * implicit free list + boundry tags +  first fit 
 * 隐式空闲链表 + 边界标记 + 首次适配
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

/* $begin mallocmacros */
/* Basic constants and macros */
#define WSIZE       4       /* Word and header/footer size (bytes) */ //line:vm:mm:beginconst
#define DSIZE       8       /* Double word size (bytes) */
#define CHUNKSIZE  (1<<12)  /* Extend heap by this amount (bytes) */  //line:vm:mm:endconst 

#define MAX(x, y) ((x) > (y)? (x) : (y))  

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc)  ((size) | (alloc)) //line:vm:mm:pack

/* Read and write a word at address p */
#define GET(p)       (*(unsigned int *)(p))            //line:vm:mm:get
#define PUT(p, val)  (*(unsigned int *)(p) = (val))    //line:vm:mm:put
#define GETP(pp)     (*(char **)(pp))            //line:vm:mm:get
#define PUTP(pp, p)  (*(char **)(pp) = (p))    //line:vm:mm:put

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)  (GET(p) & ~0x7)                   //line:vm:mm:getsize
#define GET_ALLOC(p) (GET(p) & 0x1)                    //line:vm:mm:getalloc

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp)       ((char *)(bp) - 5 * WSIZE)                      //line:vm:mm:hdrp
#define FTRP(bp)       ((char *)(bp) + GET_SIZE(HDRP(bp)) - 3 * DSIZE) //line:vm:mm:ftrp
#define PREV(bp)       ((char *)(bp) - 2 * DSIZE) 
#define NEXT(bp)       ((char *)(bp) - 1 * DSIZE) 

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp)  ((char *)(bp) + GET_SIZE(HDRP(bp))) //line:vm:mm:nextblkp
#define PREV_BLKP(bp)  ((char *)(bp) - GET_SIZE(((char *)(bp) - 3 * DSIZE))) //line:vm:mm:prevblkp
#define PREV_FREE_BLKP(bp)  (GETP(PREV(bp))) //line:vm:mm:prev free blkp
#define NEXT_FREE_BLKP(bp)  (GETP(NEXT(bp))) //line:vm:mm:next free blkp
/* $end mallocmacros */

/* Global variables */
static char *heap_listp = 0;  /* Pointer to first block */  
// static char *rover = 0;  /* Pointer to first block */  
// static int count = 4;

/* Function prototypes for internal helper routines */
static void insert(void *bp);
static void delete(void *bp);
static void *extend_heap(size_t words);
static void *place(void *bp, size_t asize);
static void *find_fit(size_t asize);
static void *coalesce(void *bp);
static void printblock(void *bp); 
static void checkheap(int verbose);
static void checkblock(void *bp);

/*  
 * mm_init - Called when a new trace starts.
 */
int mm_init(void) {  
    /* Create the initial empty heap */
    if ((heap_listp = mem_sbrk(6*DSIZE)) == (void *)-1) //line:vm:mm:begininit
        return -1;
    PUT(heap_listp, 0);                          /* Alignment padding */
    heap_listp += (3*DSIZE);
    PUT(HDRP(heap_listp), PACK(3 * DSIZE, 1)); /* Prologue header */ 
    PUTP(PREV(heap_listp), NULL);                /* prev */
    PUTP(NEXT(heap_listp), NEXT_BLKP(heap_listp));/* next */
    PUT(FTRP(heap_listp), PACK(3 * DSIZE, 1)); /* Prologue footer */ 
    PUT(HDRP(NEXT_BLKP(heap_listp)), PACK(0, 1));     /* Epilogue header */
    PUTP(PREV(NEXT_BLKP(heap_listp)), heap_listp);/* next */
    PUTP(NEXT(NEXT_BLKP(heap_listp)), NULL); /* Prologue footer */ 
    // rover = heap_listp;
    
    /* Extend the empty heap with a free block of CHUNKSIZE bytes */
    if (extend_heap(CHUNKSIZE/WSIZE) == NULL) 
        return -1;
    return 0;
}

/*
 * malloc - Allocate a block by incrementing the brk pointer.
 *      Always allocate a block whose size is a multiple of the alignment.
 */
void *malloc(size_t size) {
    size_t asize;      /* Adjusted block size */
    size_t extendsize; /* Amount to extend heap if no fit */
    char *bp;      

    /* $end mmmalloc */
    if (heap_listp == 0){
        mm_init();
    }
    /* $begin mmmalloc */
    /* Ignore spurious requests */
    if (size == 0) {
        return NULL;
    }

    /* Adjust block size to include overhead and alignment reqs. */
    asize = DSIZE * ((size + (3 * DSIZE) + (DSIZE-1)) / DSIZE); //line:vm:mm:sizeadjust3

    /* 
    if (size <= DSIZE)                                          //line:vm:mm:sizeadjust1
        asize = 2*DSIZE;                                        //line:vm:mm:sizeadjust2
    else
        asize = DSIZE * ((size + (DSIZE) + (DSIZE-1)) / DSIZE); //line:vm:mm:sizeadjust3 
    */

    /* Search the free list for a fit */
    if ((bp = find_fit(asize)) != NULL) {  //line:vm:mm:findfitcall
        bp = place(bp, asize);                 //line:vm:mm:findfitplace
        return bp;
    }

    /* No fit found. Get more memory and place the block */
    extendsize = MAX(asize,CHUNKSIZE);                 //line:vm:mm:growheap1
    if ((bp = extend_heap(extendsize/WSIZE)) == NULL)  
        return NULL;                                  //line:vm:mm:growheap2
    bp = place(bp, asize);                                 //line:vm:mm:growheap3
    return bp;
}

/*
 * free - We don't know how to free a block.  So we ignore this call.
 *      Computers have big memories; surely it won't be a problem.
 */
void free(void *bp) {
    
    if (bp == 0) 
        return;

    size_t size = GET_SIZE(HDRP(bp));

    if (heap_listp == 0){
        mm_init();
    }

    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    coalesce(bp);
}

/*
 * coalesce - Boundary tag coalescing. Return ptr to coalesced block
 */
static void *coalesce(void *bp) {
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));
    
    
    if (prev_alloc && next_alloc) {            /* Case 1 */
        insert(bp);
        return bp;
    }

    else if (prev_alloc && !next_alloc) {      /* Case 2 */
        delete(NEXT_BLKP(bp));
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size,0));
        insert(bp);
        
    }

    else if (!prev_alloc && next_alloc) {      /* Case 3 */
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
        
    }

    else {                                     /* Case 4 */
        delete(NEXT_BLKP(bp));
        size += GET_SIZE(HDRP(PREV_BLKP(bp))) + 
            GET_SIZE(FTRP(NEXT_BLKP(bp)));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
        
    }

    /* 防止rover指向被合并的空闲块 */
    // if ((rover > (char *)bp) && (rover < NEXT_BLKP(bp))) 
    //     rover = bp;

    return bp;
}

/* 
 * extend_heap - Extend heap with free block and return its block pointer
 */
static void *extend_heap(size_t words) {
    char *bp, *prevp;
    size_t size;

    /* Allocate an even number of words to maintain alignment */
    size = (words % 2) ? (words+1) * WSIZE : words * WSIZE; //line:vm:mm:beginextend
    if ((long)(bp = mem_sbrk(size)) == -1)  
        return NULL;                                        //line:vm:mm:endextend

    /* Initialize free block header/footer and the epilogue header */
    prevp = PREV_FREE_BLKP(bp);
    PUT(HDRP(bp), PACK(size, 0));         /* Free block header */   //line:vm:mm:freeblockhdr
    PUT(FTRP(bp), PACK(size, 0));         /* Free block footer */   //line:vm:mm:freeblockftr
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); /* New epilogue header */ //line:vm:mm:newepihdr
    PUTP(NEXT(prevp), NEXT_BLKP(bp));
    PUTP(PREV(NEXT_BLKP(bp)), prevp);
    PUTP(NEXT(NEXT_BLKP(bp)), NULL);
    
    /* Coalesce if the previous block was free */
    return coalesce(bp);                                          //line:vm:mm:returnblock
}

/* 
 * place - Place block of asize bytes at start of free block bp 
 *         and split if remainder would be at least minimum block size
 */
static void *place(void *bp, size_t asize) {
    size_t csize = GET_SIZE(HDRP(bp));   

    if ((csize - asize) >= (4*DSIZE)) { 
        PUT(HDRP(bp), PACK(csize-asize, 0));
        PUT(FTRP(bp), PACK(csize-asize, 0));
        bp = NEXT_BLKP(bp);
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
    }
    else { 
        PUT(HDRP(bp), PACK(csize, 1));
        PUT(FTRP(bp), PACK(csize, 1));
        delete(bp);
    }
    return bp;
}
/* $end mmplace */

/* 
 * find_fit - Find a fit for a block with asize bytes 
 */
static void *find_fit(size_t asize) {
    /* next-fit search */
    // char *oldrover = rover;
    char *rover;

    // for ( ; GET_SIZE(HDRP(rover)) > 0; rover = NEXT_FREE_BLKP(rover)) {
    //     if (asize <= GET_SIZE(HDRP(rover))) {
    //         return rover;
    //     }
    // }

    // for (rover = heap_listp; rover < oldrover; rover = NEXT_FREE_BLKP(rover)) {
    //     if (asize <= GET_SIZE(HDRP(rover))) {
    //         return rover;
    //     }
    // }

    for (rover = heap_listp; GET_SIZE(HDRP(rover)) > 0; rover = NEXT_FREE_BLKP(rover)) {
        if (asize <= GET_SIZE(HDRP(rover))) {
            return rover;
        }
    }
    return NULL; /* No fit */
}

static void insert(void *bp) {
    char *prevp;
    for (prevp = heap_listp; NEXT_FREE_BLKP(prevp) != NULL ; prevp = NEXT_FREE_BLKP(prevp)) {
        if((prevp < (char *)bp && (char *)bp < NEXT_FREE_BLKP(prevp))) {
            PUTP(PREV(NEXT_FREE_BLKP(prevp)), bp);
            PUTP(NEXT(bp), NEXT_FREE_BLKP(prevp));
            PUTP(NEXT(prevp), bp);
            PUTP(PREV(bp), prevp);
            break;
        }
    }
}

static void delete(void *bp) {
    char *prevp = PREV_FREE_BLKP(bp);
    char *nextp = NEXT_FREE_BLKP(bp);
    PUTP(NEXT(prevp), nextp);
    PUTP(PREV(nextp), prevp);
    PUTP(NEXT(bp), NULL);
    PUTP(PREV(bp), NULL);
}

static void printblock(void *bp) 
{
    size_t hsize, halloc, fsize, falloc;

    checkheap(0);
    hsize = GET_SIZE(HDRP(bp));
    halloc = GET_ALLOC(HDRP(bp));  
    fsize = GET_SIZE(FTRP(bp));
    falloc = GET_ALLOC(FTRP(bp));  

    if (hsize == 0) {
        printf("%p: header: [%4ld:%c] prev:[%11p] next[%11p]\n", bp, 
           hsize, (halloc ? 'a' : 'f'), 
           PREV_FREE_BLKP(bp), NEXT_FREE_BLKP(bp));
        // printf("%p: EOL\n", bp);
        return;
    }

    printf("%p: header: [%4ld:%c] prev:[%11p] next[%11p] footer: [%4ld:%c]\n", bp, 
           hsize, (halloc ? 'a' : 'f'), 
           PREV_FREE_BLKP(bp), 
           NEXT_FREE_BLKP(bp),
           fsize, (falloc ? 'a' : 'f')); 
}

static void checkblock(void *bp) 
{
    if ((size_t)bp % 8)
        printf("Error: %p is not doubleword aligned\n", bp);
    if (GET(HDRP(bp)) != GET(FTRP(bp)))
        printf("Error: header does not match footer\n");
}

/* 
 * checkheap - Minimal check of the heap for consistency 
 */
void checkheap(int verbose) 
{
    char *bp = heap_listp;

    if (verbose)
        printf("Heap (%p):\n", heap_listp);

    if ((GET_SIZE(HDRP(heap_listp)) != 3 * DSIZE) || !GET_ALLOC(HDRP(heap_listp)))
        printf("Bad prologue header\n");
    checkblock(heap_listp);

    for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)) {
        if (verbose) 
            printblock(bp);
        checkblock(bp);
    }

    if (verbose)
        printblock(bp);
    if ((GET_SIZE(HDRP(bp)) != 0) || !(GET_ALLOC(HDRP(bp))))
        printf("Bad epilogue header\n");
}

/*
 * realloc - Change the size of the block by mallocing a new block,
 *      copying its data, and freeing the old block.  I'm too lazy
 *      to do better.
 */
void *realloc(void *oldptr, size_t size)
{
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
  oldsize = GET_SIZE(HDRP(oldptr));
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