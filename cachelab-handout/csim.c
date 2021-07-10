#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include "cachelab.h"

#define LOAD 'L'
#define STORE 'S'
#define MODIFY 'M'
#define HIT 1
#define MISS 2
#define EVICTION 4
#define MAXLINE 100

//命令行参数
int getopt(int argc, char * const argv[], const char *optstring);

extern char *optarg;
extern int optind, opterr, optopt;

/* LRU(Least Recently used)链表的节点 */
typedef struct Node {//数据类型由大到小排，减少因对齐产生的填充块
    //char *data;
    struct Node *prev;
    struct Node *next;
    unsigned long tag;
    char valid;
    char dirty;
} Node;

/* LRU(Least Recently used)的双向链表 */
typedef struct LRU {
    Node *head;
    Node *tail;
    int capacity;
    int size;
} LRU;

/* 缓存 */
typedef struct Cache {
    LRU **lrus;
    int s;
    int E;
    int b;
} Cache;

/* 装载和存储方法 */
int load(Cache * cache, unsigned long address);
int store(Cache * cache, unsigned long address);

/* LRU链表的增删改方法 */
void prepend(LRU *lru, Node *src);
void insert(LRU *lru, Node *dest, Node *src);
Node *delete(LRU *lru, Node *dest);
//void update(LRU *lru, Node *dest);

/* Cache, Set,Node和地址构造器 */
Cache *cache_constructor(int s, int E, int b);
LRU *LRU_constructor(int E);
Node *Node_constructor(unsigned long tag, char valid, char dirty);

/* 读取地址中的标签tag，LRU链表的下标LRU_index */
unsigned long get_tag(Cache * cache, unsigned long address);
unsigned long get_LRU_index(Cache * cache, unsigned long address);
//unsigned long get_block_offset(Cache * cache, unsigned long address);

/* 释放空间 */
void free_cache(Cache *cache);
void free_lru(LRU *lru);
void free_node(Node *node);

int main(int argc, char **argv)
{   
    //command line
    // • -h: Optional help flag that prints usage info
    // • -v: Optional verbose flag that displays trace info
    // • -s <s>: Number of set index bits (S = 2 s is the number of sets)
    // •-E <E>: Associativity (number of lines per set)
    // • -b <b>: Number of block bits (B = 2b is the block size)
    // • -t <tracefile>: Name of the valgrind trace to replay

    int option, verbose = 0;
    int s = 0, E = 0, b = 0;
    char *filepath;
    int hit_count = 0, miss_count = 0, eviction_count = 0;
    char operation;
    unsigned long address;
    int length;
    int load_return, store_return;

    // while (--argc > 0 && (*++argv)[0] == '-') {
    //     while ((option = *++argv[0])) {
    //         switch (option) {
    //             case 'h':
    //                 printf("Usage: ./csim-ref [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n");
    //                 break;
    //             case 'v':
    //                 verbose = 1;
    //                 break;
    //             case 's':
    //                 s = atoi(*++argv);
    //                 break;
    //             case 'E':
    //                 E = atoi(*++argv);
    //                 break;
    //             case 'b':
    //                 b = atoi(*++argv);
    //                 break;
    //             case 't':
    //                 filepath = *++argv;
    //                 break;
    //             default:
    //                 printf("find: illegal option %c\n", option);
    //                 argc = 0;
    //                 break;
    //         }
    //         if (option == 's' || option == 'E' || option == 'b' || option == 't') {
    //             --argc;
    //             break;      
    //         }
    //     }
    // }

    // char opt;
    while ((option = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
        switch (option) {
            case 'h':
                printf("Usage: ./csim [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n");
                break;
            case 'v':
                verbose = 1;
                break;
            case 's':
                s = atoi(optarg);
                break;
            case 'E':
                printf("%s", optarg);
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                filepath = optarg;
                break;
            default:
                // opt = (char) optopt;
                exit(1);
            // case ':':
            //     opt = (char) optopt;
            //     printf("Error: option -%c needs option argment\n", opt);
                // exit(1);
        }
    }
    Cache *cache = cache_constructor(s, E, b);
    FILE *fp = fopen(filepath, "r");
    FILE *write_file = fopen("transpose_trace_verbose.txt", "a+");
    char c;
    while(fscanf(fp, "%c", &c) == 1) { //检查行首是否有一个空格
        if (c == 'I') {
            fscanf(fp, "%*[^\n]%*c");
            continue;
        }
    
        if (c != ' ' || fscanf(fp, "%c %lx,%d%*c", &operation, &address, &length) != 3) {
            exit(1);
        }
        
        switch (operation) {
            case LOAD:
                load_return = load(cache, address);
                store_return = 0;
                break;
            case STORE:
                load_return = 0;
                store_return = store(cache, address);
                break;
            case MODIFY:
                load_return = load(cache, address);
                store_return = store(cache, address); 
                break;
        } 
        if (verbose) {
            printf("%c %lx,%d", operation, address, length);
            ((load_return & 2) >> 1) ? printf(" miss") : 0;
            ((store_return & 2) >> 1) ? printf(" miss") : 0;

            ((load_return & 4) >> 2) ? printf(" eviction") : 0;
            ((store_return & 4) >> 2) ? printf(" eviciton") : 0;

            (load_return & 1) ? printf(" hit") : 0;
            (store_return & 1) ? printf(" hit") : 0;
            
            printf("\n");
        }
        if (verbose) {
            fprintf(write_file, "%c %lx,%d", operation, address, length);
            ((load_return & 2) >> 1) ? fprintf(write_file, " miss") : 0;
            ((store_return & 2) >> 1) ? fprintf(write_file, " miss") : 0;

            ((load_return & 4) >> 2) ? fprintf(write_file, " eviction") : 0;
            ((store_return & 4) >> 2) ? fprintf(write_file, " eviciton") : 0;

            (load_return & 1) ? fprintf(write_file, " hit") : 0;
            (store_return & 1) ? fprintf(write_file, " hit") : 0;
            
            fprintf(write_file, "\n");
        }
        hit_count += (load_return & 1);
        hit_count += (store_return & 1);
        miss_count += ((load_return & 2) >> 1);
        miss_count += ((store_return & 2) >> 1);
        eviction_count += ((load_return & 4) >> 2);
        eviction_count += ((store_return & 4) >> 2);

    }
      
    fclose(fp);
    free_cache(cache);
    printSummary(hit_count, miss_count, eviction_count);


    //test node
    // Node *node = Node_constructor(1, 1, 0);
    // printf("tag=%lu, valid=%d, dirty=%d\n", node->tag, node->valid, node->dirty);
    // free_node(node);
    // node = NULL;

    //test lru
    // LRU *lru = LRU_constructor(4);
    // prepend(lru, Node_constructor(1, 1, 0));
    // prepend(lru, Node_constructor(2, 1, 0));
    // prepend(lru, Node_constructor(3, 1, 0));
    // prepend(lru, Node_constructor(4, 1, 0));
    // prepend(lru, Node_constructor(5, 1, 0));

    // free_node(delete(lru, lru->tail->prev));
    // prepend(lru, delete(lru, lru->tail->prev));
    // for (Node *np = lru->head->next; np != lru->tail; np = np->next) {
    //     printf("tag=%lu, valid=%d, dirty=%d, size=%d\n", np->tag, np->valid, np->dirty, lru->size);
    // }

    // free_lru(lru);
    

    //test cache
    // Cache *cache = cache_constructor(4, 1, 4);
    // LRU **lrus;
    // LRU **end = cache->lrus + 16;
    // int count = 0;
    // for (lrus = cache->lrus; lrus < end; lrus += 2) {
    //     *lrus = LRU_constructor(4);
    //     prepend(*lrus, Node_constructor(1, 1, 0));
    //     prepend(*lrus, Node_constructor(2, 1, 0));
    //     prepend(*lrus, Node_constructor(3, 1, 0));
    // }
    
    // for (lrus = cache->lrus; lrus < end; lrus++) {
    //     printf("cout=%d, %d\n", count, *lrus == NULL);
    //     count++;
    // }
    // free_cache(cache);


    //test fcanf
    // char *filepath = "traces/yi.trace";
    // FILE *fp = fopen(filepath, "r");
    // char c, operation;
    // unsigned long address;
    // unsigned length;
    // // int operation;
    // // unsigned long address;
    // // unsigned length;
    // while(fscanf(fp, "%c", &c) == 1) { //检查行首是否有一个空格
    //     if (c == 'I') {
    //         fscanf(fp, "%*[^\n]%*c");
    //         continue;
    //     }
        
    //     if (c != ' ' && fscanf(fp, "%c %lx,%d%*c", &operation, &address, &length) != 3) {
    //             exit(1);
    //     }
    // }
    return 0;
}

Cache *cache_constructor(int s, int E, int b) {
    Cache *cache = (Cache *) malloc(sizeof(Cache));
    long nmenb = pow(2, s);
    
    cache->lrus = (LRU **) calloc(nmenb, sizeof(LRU *));
    cache->s = s;
    cache->E = E;
    cache->b = b;
    
    return cache;
}

LRU *LRU_constructor(int E) {
    LRU *lru = (LRU *) malloc(sizeof(LRU));
    
    lru->head = Node_constructor(0, 0, 0);
    lru->tail = Node_constructor(0, 0, 0);
    
    lru->head->next = lru->tail;
    lru->head->prev = NULL;
    
    lru->tail->prev = lru->head;
    lru->tail->next = NULL;
    
    lru->capacity = E;
    lru->size = 0;
    return lru;
}

Node *Node_constructor(unsigned long tag, char valid, char dirty) {
    Node *node = (Node *) malloc(sizeof(Node));
    
    node->tag = tag;
    node->valid = valid;
    node->dirty = dirty;
    
    return node;
}

void free_cache(Cache *cache) {
    LRU **lrus1 = cache->lrus;
    LRU **end = cache->lrus + (long) pow(2, cache->s);

    for (; lrus1 != end; lrus1++) {
        if (*lrus1)
            free_lru(*lrus1);
    }

    free(cache->lrus);
    free(cache);
}

void free_lru(LRU *lru){
    Node *node1;
    Node *node2;

    for(node1 = lru->head; node1 != NULL; node1 = node2) {
        node2 = node1->next;
        free_node(node1);
    }

    free(lru);
}

void free_node(Node *node) {
    free(node);
}

int load(Cache * cache, unsigned long address) {
    unsigned long tag = get_tag(cache, address);
    unsigned long lru_index = get_LRU_index(cache, address);
    //unsigned long block_offset = get_block_offset(cache, address);

    LRU **lrus = cache->lrus + lru_index;
    if (*lrus == NULL) {
        *lrus = LRU_constructor(cache->E);
        prepend(*lrus, Node_constructor(tag, 1, 0));
        return MISS;
    }

    LRU *lru = *lrus;
    Node *node = lru->head->next;
    while (node != lru->tail) {
        if (node->valid && node->tag == tag) {
            if (node != lru->head->next) {
                prepend(lru, delete(lru, node));
            }
            return HIT;
        }
        node = node->next;
    }

    if (lru->size >= lru->capacity) {
        free_node(delete(lru, lru->tail->prev));
        // if (lru->tail->prev->dirty) {
        //     //write back
        // }
        prepend(lru, Node_constructor(tag, 1, 0));
        return MISS + EVICTION;
    }
    else {
        prepend(lru, Node_constructor(tag, 1, 0));
        return MISS;
    }
}

int store(Cache * cache, unsigned long address) {
    unsigned long tag = get_tag(cache, address);
    unsigned long lru_index = get_LRU_index(cache, address);
    //unsigned long block_offset = get_block_offset(cache, address);

    LRU **lrus = cache->lrus + lru_index;
    if (*lrus == NULL) {
        *lrus = LRU_constructor(cache->E);
        prepend(*lrus, Node_constructor(tag, 1, 0));
        return MISS;
    }

    LRU *lru = *lrus;
    Node *node = lru->head->next;
    while (node != lru->tail) {
        if (node->valid && node->tag == tag) {
            node->dirty = 1;
            if (node != lru->head->next) {
                prepend(lru, delete(lru, node));
            }
            return HIT;
        }
        node = node->next;
    }

    if (lru->size >= lru->capacity) {
        free_node(delete(lru, lru->tail->prev));
        // if (lru->tail->prev->dirty) {
        //     //write back
        // }
        prepend(lru, Node_constructor(tag, 1, 0));
        return MISS + EVICTION;
    }
    else {
        prepend(lru, Node_constructor(tag, 1, 0));
        return MISS;
    }
}

void prepend(LRU *lru, Node *src) {
    if (lru->size >= lru->capacity)
        return;

    Node *next = lru->head->next;
    lru->head->next = src;
    src->prev = lru->head;

    src->next = next;
    next->prev = src;

    lru->size++;
}

void insert(LRU *lru, Node *dest, Node *src) {
    if (lru == NULL || dest == NULL || src == NULL) {
        return;
    }

    Node *prev = dest->prev;
    prev->next = src;
    src->prev = prev;
    src->next = dest;
    dest->prev = src;
    lru->size++;
}

Node *delete(LRU *lru, Node *dest) {
    Node *prev = dest->prev;
    Node *next = dest->next;
    prev->next = next;
    next->prev = prev;
    lru->size--;
    return dest;
}

unsigned long get_tag(Cache * cache, unsigned long address) {
    return address >> (cache->s + cache->b);
}

unsigned long get_LRU_index(Cache * cache, unsigned long address) {
    return (address >> cache->b) & ((1 << cache->s) - 1);
}