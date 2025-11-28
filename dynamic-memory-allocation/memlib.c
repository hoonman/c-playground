#include <stdio.h>

static char* mem_heap; // points to first byte of heap
static char* mem_brk; // points to last byte of heap plus 1
static char* mem_max_addr; // max legal heap addr plus 1

void mem_init(void) {
    mem_heap = (char*) Malloc(MAX_HEAP); // allocate large fixed block of memory from the system
    mem_brk = (char*) mem_heap;
    mem_max_addr = (char*)(mem_heap + MAX_HEAP);
}

// mm_init function: initializes the allocator, returning 0 if sucecssful and -1 otherwise. 
// mm_malloc
// mm_free 

// unused padding word -> prologue block -> ... -> epilogue
// prologue block are zero or more regular blocks that are created by calls to malloc or free 
// epilogue block: zero-size allocated block that consists of only a header. 
// they eliminate edge conditions during coalescing.

// manipulating headers and footers in the free list can be trouble 
// pack 

void *mem_sbrk(int incr)
{
    char *old_brk = mem_brk;

    if ( (incr < 0) || ((mem_brk + incr) > mem_max_addr)) {
        errno = ENOMEM;
        fprintf(stderr, "ERROR: mem_sbrk failed. Ran out of memory...\n");
        return (void *)-1;
    }
    mem_brk += incr;
    return (void *)old_brk;
}