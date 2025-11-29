
#define WSIZE 4 /* Word and header/footer size (bytes) */
#define DSIZE 8 /* Double word size (bytes) */
#define CHUNKSIZE (1<<12) /* Extend heap by this amount (bytes) */
// this is 4096 bytes

#define MAX(x, y) ((x) > (y)? (x) : (y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

int mm_init(void) {
    // create the initial empty heap
    if ((heap_listp = mem_sbrk(4*WSIZE)) == (void*)-1) { // 4 words
        return -1; //something failed
    }

    PUT(heap_listp, 0); // alignment padding
    PUT(heap_listp + (1*WSIZE), PACK(DSIZE, 1)); // prologue header
    PUT(heap_listp + (2*WSIZE), PACK(DSIZE, 1)) // prologue footer
    PUT(heap_listp + (3*WSIZE), PACK(0, 1)) // epilogue header
    heap_listp += (2*WSIZE); // im guessing this moves heap_listp to point to the prologue block 

    // extend the empty heap with a free block of chunksize bytes
    if (extend_heap(CHUNKSIZE / WSIZE) == NULL) { // 1024 words 
        return -1;
    }
    return 0;
}

// two scenarios to use:
// 1. when the heap is initialized
// 2. when mm_malloc is unable to find a suitable fit
static void *extend_heap(size_t words) { 
    char* bp; // block pointers
    size_t size;

    // allocate an even number of words to maintain alignment
    if (words % 2 == 0) {
        size = words * WSIZE;
    } else {
        size = (words + 1) * WSIZE;
    }

    if ((long)(bp = mem_sbrk(size)) == -1)
        return NULL;

    // initialize free block header / footer and the epilogue header
    PUT(HDRP(bp), PACK(size, 0)); // free block header
    // assign the new block pointer size of the newly created block and 0 (for unallcoated)
    // HDRP moves bp backwards one word, and then put assigns bp the new block size as well as set to 0 for "not allocated"  
    PUT(FTRP(bp), PACK(size, 0)); // free block footer
    // FTRP moves bp towards the footer and assigns the footer with the size of the free block and 0 for unallocated
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); // new epilogue header 
    // PACK(0, 1) -- 0 because we have no payload for the epilogue header
    // 1 for allocated -- this marks the end of the heap.

    return coalesce(bp); // covers an edge case where previous block might have been free. 
}

//mm_free: frees the current block pointer AND calls coalesce in case there are adjacent free blocks
// also need to adjust the footer 
// check the footer of the prev block and the header of the next block
// we might not need to check the above because we have coalesce that does things for us 
void mm_free(void* bp) {
    size_t size = GET_SIZE(HDRP(bp));

    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));

    coalesce(bp);
    return;
}

//coalesce
void coalesce(void* bp) {
    // get prev block footer
    // get next block header
    void* prev_bp = PREV_BLKP(bp);
    void* next_bp = NEXT_BLKP(bp);
    size_t prev_footer = GET_SIZE(FTRP(prev_bp));
    size_t next_header = GET_SIZE(HDRP(next_bp));
    size_t total_size;

    if (prev_footer != 0 && next_header != 0) {
        return bp;
    }

    if (prev_footer != 0 && next_header == 0) {
        total_size = GET_SIZE(next_bp) + GET_SIZE(bp);
        PUT(HDRP(bp), PACK(total_size, 0));
        PUT(FTRP(next_bp), PACK(total_size, 0));
    }

    if (prev_footer == 0 && next_header != 0) {
        // calculate total size prevfooter size + bp size 
        // update the header of the prev block to reflect total size. and flip allocation bit to 0
        // update the footer of the bp to reflect the total size and flip allocation bit to 0
        total_size = GET_SIZE(prev_bp) + GET_SIZE(bp);
        PUT(HDRP(prev_bp), PACK(total_size, 0));
        // PUT(FTRP(prev_bp), PACK(total_size, 0));
        // PUT(FTRP(bp), PACK(total_size, 0));
        PUT(FTRP(bp), PACK(total_size, 0));
        bp = PREV_BLKP(bp);
    }


    if (prev_footer == 0 && next_header == 0) {
        total_size = GET_SIZE(prev_bp) + GET_SIZE(bp) + GET_SIZE(next_bp);
        PUT(HDRP(prev_bp), PACK(total_size, 0));
        PUT(FTRP(next_bp), PACK(total_size, 0));
        bp = PREV_BLKP(bp);
    }

    return bp;
}
//mm_malloc

void* mm_malloc(size_t size) {
    // return bp that is to the next
    // we must search for a free block or do we have a pointer we can use ? 
    size_t new_size;
    size_t amount_to_extend; // amount to extend heap if we have no fit. 
    char * bp;

    if (size == 0) {
        return NULL;
    }

    if (size <= DSIZE) {
        new_size = 2 * DSIZE;
    } else {
        new_size = DSIZE * ((size + (DSIZE) + (DSIZE - 1)) / DSIZE);
    }

    if ((bp = find_fit(new_size)) != NULL) {
        place(bp, new_size);
        return bp;
    }

    // no fit found get more memory and place the block
    amount_to_extend = MAX(new_size, CHUNKSIZE);
    if ((bp = extend_heap(amount_to_extend / WSIZE) == NULL)) return NULL;
    place(bp, new_size);
    return bp;

}

static void* find_fit(size_t size) {
    // we need to find a block that is header size is equal or less than. perform this in a while loop? 

    char* bp = heap_listp;

    while (bp != NULL) {
        if (GET_SIZE(HDRP(bp)) >= size) {
            return bp;
        }
        bp = NEXT_BLKP(bp);
    }
    return NULL;
}

static void* find_fit(size_t size) {
    void* bp;
    // epilogue size is 0 so we will loop UNTIL get_size(hdrp(bp)) is greater than 0. all the other blocks would have sizes but not the epilogue block ! 
    for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)) {
        if (!GET_ALLOC(HDRP(bp)) && (size <= GET_SIZE(HDRP(bp)))) {
            return bp;
        }
    }
    return NULL;
}

static void place(void *bp, size_t size) {
    // place the requested block at the beginning of the free block 
    // split only if the size of the remainder would equal or exceed the minimum block size

    // update bp's header and size
    // input size is already aligned. so we can just use the input size
    size_t remainder = size - GET_SIZE(HDRP(bp));
    PUT(HDRP(bp), PACK(size, 1)); // update bp header
    if (remainder >= (2*DSIZE)) { // 2 * DSIZE accounting for the header and fotter 
        // how do we find the new footer for the allocated block and header for the new free block? 
        // bp = FTRP(bp);
        PUT(FTRP(bp), PACK(size, 1));
        bp = NEXT_BLKP(bp);
        PUT(HDRP(bp), PACK(remainder, 0));
        PUT(FTRP(bp), PACK(remainder, 0));
    } else if (remainder == 0) {
        bp = FTRP(bp);
        PUT(FTRP(bp), PACK(size, 1));
    }

}