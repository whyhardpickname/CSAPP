#include<stdlib.h>

#define MAX_HEAP (20 * (1 << 20)) /* 20MB */


static char *mem_heap; /* 指向堆开头 */
static char *mem_brk; /* 指向堆末尾 + 1 */
static char *mem_max_addr; /* 堆最大合法地址 + 1 */

void mem_init(void){
    mem_heap = (char *) malloc(MAX_HEAP);
    mem_brk = (char *) mem_heap;
    mem_max_addr = (char *) (mem_heap + MAX_HEAP); /* 上限 */
}

void *mem_sbrk(int incr) {
    char *old_mem_brk = mem_brk;

    if (incr < 0 || (mem_heap + incr) >= mem_max_addr) {
        return (void *) -1;
    }
    
    mem_brk += incr;
    return old_mem_brk;
}
