#include "allocator.h"
#include <stdio.h>
#include <string.h>

#define HEAP_CAPACITY (64 * 1024)
#define ALIGNMENT 8
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

typedef struct block_header {
    size_t size;
    int is_free;
    struct block_header *next;
    struct block_header *prev;
} block_header_t;

#define HEADER_SIZE ALIGN(sizeof(block_header_t))

static uint8_t heap_pool[HEAP_CAPACITY];
static block_header_t *free_list_head = NULL;
static AllocationStrategy current_strategy = STRATEGY_FIRST_FIT;

static void free_list_add(block_header_t *block) {
    block->is_free = 1;
    block->next = free_list_head;
    block->prev = NULL;
    if (free_list_head != NULL) {
        free_list_head->prev = block;
    }
    free_list_head = block;
}

static void free_list_remove(block_header_t *block) {
    block->is_free = 0;
    if (block->prev != NULL) {
        block->prev->next = block->next;
    } else {
        free_list_head = block->next;
    }
    if (block->next != NULL) {
        block->next->prev = block->prev;
    }
    block->next = NULL;
    block->prev = NULL;
}

void heap_init(AllocationStrategy strategy) {
    current_strategy = strategy;
    memset(heap_pool, 0, HEAP_CAPACITY);

    block_header_t *initial_block = (block_header_t *)heap_pool;
    initial_block->size = HEAP_CAPACITY - HEADER_SIZE;
    initial_block->is_free = 1;
    initial_block->next = NULL;
    initial_block->prev = NULL;

    free_list_head = initial_block;
}

static void split_block(block_header_t *block, size_t requested_size) {
    size_t total_block_size = block->size;
    if (total_block_size >= requested_size + HEADER_SIZE + ALIGNMENT) {
        size_t remaining_size = total_block_size - requested_size - HEADER_SIZE;
        block_header_t *new_block = (block_header_t *)((uint8_t *)block + HEADER_SIZE + requested_size);
        new_block->size = remaining_size;
        
        block->size = requested_size;

        free_list_remove(block);
        free_list_add(new_block);
        free_list_add(block);
        free_list_remove(block);
    } else {
        free_list_remove(block);
    }
}

void *my_malloc(size_t size) {
    if (size == 0) return NULL;

    size_t adjusted_size = ALIGN(size);
    block_header_t *current = free_list_head;
    block_header_t *selected_block = NULL;

    if (current_strategy == STRATEGY_FIRST_FIT) {
        while (current != NULL) {
            if (current->size >= adjusted_size) {
                selected_block = current;
                break;
            }
            current = current->next;
        }
    } else if (current_strategy == STRATEGY_BEST_FIT) {
        size_t min_diff = (size_t)-1;
        while (current != NULL) {
            if (current->size >= adjusted_size) {
                size_t diff = current->size - adjusted_size;
                if (diff < min_diff) {
                    min_diff = diff;
                    selected_block = current;
                }
            }
            current = current->next;
        }
    } else if (current_strategy == STRATEGY_WORST_FIT) {
        size_t max_size = 0;
        while (current != NULL) {
            if (current->size >= adjusted_size) {
                if (current->size > max_size) {
                    max_size = current->size;
                    selected_block = current;
                }
            }
            current = current->next;
        }
    }

    if (selected_block == NULL) return NULL;

    split_block(selected_block, adjusted_size);
    return (void *)((uint8_t *)selected_block + HEADER_SIZE);
}

void my_free(void *ptr) {
    if (ptr == NULL) return;

    block_header_t *block = (block_header_t *)((uint8_t *)ptr - HEADER_SIZE);
    free_list_add(block);

    uint8_t *heap_end = heap_pool + HEAP_CAPACITY;
    block_header_t *curr = (block_header_t *)heap_pool;
    while ((uint8_t *)curr < heap_end) {
        block_header_t *neighbor = (block_header_t *)((uint8_t *)curr + HEADER_SIZE + curr->size);
        if ((uint8_t *)neighbor < heap_end && curr->is_free && neighbor->is_free) {
            free_list_remove(curr);
            free_list_remove(neighbor);
            curr->size += HEADER_SIZE + neighbor->size;
            free_list_add(curr);
            curr = (block_header_t *)heap_pool;
            continue;
        }
        curr = neighbor;
    }
}

void *my_realloc(void *ptr, size_t size) {
    if (ptr == NULL) return my_malloc(size);
    if (size == 0) {
        my_free(ptr);
        return NULL;
    }

    block_header_t *block = (block_header_t *)((uint8_t *)ptr - HEADER_SIZE);
    if (block->size >= size) return ptr;

    void *new_ptr = my_malloc(size);
    if (new_ptr != NULL) {
        memcpy(new_ptr, ptr, block->size);
        my_free(ptr);
    }
    return new_ptr;
}

void heap_dump(void) {
    printf("\n--- HEAP DIAGNOSTICS & DUMP ---\n");
    block_header_t *curr = (block_header_t *)heap_pool;
    uint8_t *heap_end = heap_pool + HEAP_CAPACITY;
    int block_index = 0;
    size_t total_allocated = 0;
    size_t total_free = 0;
    int count_allocated = 0;
    int count_free = 0;

    while ((uint8_t *)curr < heap_end) {
        printf("Block %d: [%s] | Address: %p | Payload Size: %zu bytes\n",
               block_index++,
               curr->is_free ? "FREE" : "ALLOCATED",
               (void *)curr,
               curr->size);

        if (curr->is_free) {
            total_free += curr->size;
            count_free++;
        } else {
            total_allocated += curr->size;
            count_allocated++;
        }

        curr = (block_header_t *)((uint8_t *)curr + HEADER_SIZE + curr->size);
    }

    printf("--------------------------------\n");
    printf("Total Allocated Space: %zu bytes (%d blocks)\n", total_allocated, count_allocated);
    printf("Total Free Space:      %zu bytes (%d blocks)\n", total_free, count_free);
    printf("--------------------------------\n\n");
}