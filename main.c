#include <stdio.h>
#include "allocator.h"

int main() {
    printf("Initializing Custom Dynamic Memory Allocator (First-Fit Strategy)...\n");
    heap_init(STRATEGY_FIRST_FIT);

    printf("\nAllocating p1 (128 bytes), p2 (256 bytes), p3 (512 bytes)...\n");
    void *p1 = my_malloc(128);
    void *p2 = my_malloc(256);
    void *p3 = my_malloc(512);

    heap_dump();

    printf("Freeing p2 (256 bytes)...\n");
    my_free(p2);

    heap_dump();

    printf("Allocating p4 (100 bytes)...\n");
    void *p4 = my_malloc(100);

    heap_dump();

    printf("Cleaning up remaining memory...\n");
    my_free(p1);
    my_free(p3);
    my_free(p4);

    heap_dump();

    return 0;
}