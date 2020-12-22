#include<stdio.h>

void *malloc(size_t size);
void *memset(void *s, int c, size_t n);
void *calloc(size_t nmenb, size_t size);

int main()
{
    
    return 0;
}

void *calloc(size_t nmenb, size_t size)
{
    if (nmenb == 0 || size == 0)
    {
        return NULL;
    }
    
    void *s = malloc(nmenb * size);
    memset(s, 0, nmenb);
}