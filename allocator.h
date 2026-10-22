#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>
#include <stdint.h>

typedef enum {
    STRATEGY_FIRST_FIT,
    STRATEGY_BEST_FIT,
    STRATEGY_WORST_FIT
} AllocationStrategy;

void heap_init(AllocationStrategy strategy);
void *my_malloc(size_t size);
void my_free(void *ptr);
void *my_realloc(void *ptr, size_t size);
void heap_dump(void);

#endif