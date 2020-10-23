#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "mymalloc.h"

static char memory[MEM_SIZE];


/*
 * Function: incrementPointer()
 * Inputs: the amount of bytes to increment a pointer by in bytes
 */

Metadata *incrementPointer(Metadata *ptr, int n) {
    char *ptr2 = (char *)ptr;
    ptr2 += n;
    return (Metadata *)ptr2;
}

/*
 * Function Description: mymalloc()
 *      1. Check input validity
 *          a. Any length input < 0 is invalid
 *          b. Any length == 0 returns NULL
 *          b. If an invalid input is detected, return NULL
 *      2. Check to see if the memory array is initialized
 *          a. If not, initialize the first metadata
 *          b. If so, continue
 *      3. Traverse through memory to look for a free block big enough to hold data of length size_t
 *          a. If contiguous blocks are found as we traverse, we merge them together by altering the previous metadata's length
 *          b. The loop will iterate another time if:
 *              i.   The ptr in question is in bounds 
 *              ii.  The ptr in question is closed or is free but too small
 *      4. Decide if we have enough space
 *          a. If the ptr is out of bounds, we do not have space for the data and ERROR
 *          b. If the ptr is in bounds, then we partition this data
 *  ->  5. Partition the free block in question
 *          //! NOTE
 *           - If we have made it this far into the code, then ptr is pointing to a free block big enough for our data
 *           - Partitioning the data requires that we separate the current free block into a CLOSED block and an OPEN block (if possible)
 *              i.   We check if this is possible by merging any contiguous free blocks that occur after the current position of ptr
 *              ii.  We do this because in the case where (There is a OPEN block after the current position of ptr) && 
 *                   (The current OPEN block doesn't have enough space to allocate a metadata for the free block)
 *              iii. In the case above, we lose waste a few bytes if we don't merge contiguous blocks that occur after the OPEN block at the current position of ptr
 *          //! ENDNOTE
 *          a. We merge contiguous free blocks that occur after the current position of pointer
 *          b. We calculate if we have space to partition into 1 CLOSED block and 1 OPEN block
 *          c. Based on calculation in 4b, we change (or don't) the length value of ptr
 *          d. Based on calculation in 4b, we may or may not insert metadata to correspond to an OPEN block that occurs after the current position of ptr
 *          e. Return the current position of ptr + sizeof(Metadata)
 *              i. This is because we don't want to return the metadata itself. Rather, we want to return the pointer to the first block of data
 */
void *mymalloc(size_t length, char *file, int line) {
    //1. Input Validity Check
    if(length < 0){
        printf("Error: attemped to allocate a negative amount of memory\n");
        return NULL;
    }
    if(length == 0){
        printf("Warning: 0 bytes allocated. No memory allocated\n");
        return NULL;
    }
    //2. memory intialization check
        //Check to see if the memory array hasn't been initialized
        //This is because we should not have blocks of length 0
    if(DEBUG) printf("Step 2 ");
    Metadata *metadata = (Metadata *) memory; 
    if(metadata->length == 0) { //Memory is uninitialized
        metadata->length = MEM_SIZE - sizeof(Metadata);
        metadata->status = OPEN;
        //memory[0] = metadata;
        if(DEBUG) printf("Complete: Intialized memory\n");
    }
    if(DEBUG) printf("Complete: already intialized memory\n");
    //3. Search memory
        // while iterating through free spaces
        // if length is small enough & metadata says space is free then malloc space for it and mark as closed
        // While loop stops for following 2 conditions: 
        //      1) We found free block big enough for metadata; 
        //      2) We reach the end of memory without finding a good block
    Metadata *ptr = (Metadata *) memory;
    Metadata *prev = NULL;
    if(DEBUG) printf("Step 3 Begin\n");
    if(DEBUG) printf("Step 3 Boolean Evaluation\n");
    if(DEBUG) printf("ptr->status is: %d\t ptr->length is: %d\n", ptr->status, ptr->length);
    if(DEBUG) printf("\t Boolean 1: ptr < memory + MEM_SIZE evaluates to: %d\n", ptr < (Metadata *) (memory + MEM_SIZE));
    if(DEBUG) printf("\t Boolean 2: (ptr->status == CLOSED || (ptr->status == OPEN && ptr->length < (int)length)) to: %d\n", (ptr->status == CLOSED || (ptr->status == OPEN && ptr->length < (int)length)));
    while(ptr < (Metadata *) (memory + MEM_SIZE) && (ptr->status == CLOSED || (ptr->status == OPEN && ptr->length < (int)length))){
        //Merges contiguous free blocks
        if(DEBUG) printf("\tMade it inside while loop\n");
        prev = ptr;
        if(DEBUG) printf("\tMade it first line. Ptr = %p\n", ptr);
        ptr = incrementPointer(ptr, sizeof(Metadata) + ptr->length);
        if(DEBUG) printf("\tMade it second line, Ptr is now = %p\n", ptr);
        if(ptr < (Metadata *) (memory + MEM_SIZE) && (prev->status == OPEN && ptr->status == OPEN)){
            if(DEBUG) printf("\t\tPerforming Merge\n");
            prev->length += sizeof(Metadata) + ptr->length;
            if(DEBUG) printf("\t\tFirst line if statement reached\n");
            ptr = prev;
            if(DEBUG) printf("\tSecond line if statement reached\n");
        }
    }

    if(DEBUG) printf("Determining if we have enough space...\n");
    if(DEBUG) printf("ptr value is: %p\tmemory+MEM_SIZE is: %p\n", ptr, memory+MEM_SIZE);
    //4. Determine if we have enough space
    if(ptr >= (Metadata *) (memory + MEM_SIZE)) {
        printf("Error: Not enough memory to allocate requested amount of data\n");
        return NULL;
    }
    if(DEBUG) printf("Step 5: Partitioning and merging continuous blocks\n");
    //5. Partition
    metadata = ptr;     //*Recycling previous metadata ptr that is no longer in use as reference to the block we want to allocate data to
    //5a. Merge contiguous free blocks (again)
    while(ptr < (Metadata *) (memory + MEM_SIZE) && ptr->status == OPEN){ 
        if(DEBUG) printf("\tMade it inside while loop 5a\n");
        prev = ptr;
        ptr = incrementPointer(ptr, sizeof(Metadata) + ptr->length);
        if(ptr < (Metadata *) (memory + MEM_SIZE) && (prev->status == OPEN && ptr->status == OPEN)){
            prev->length += sizeof(Metadata) + ptr->length;
            ptr = prev;
            if(DEBUG) printf("\tMerged Data Here\n");
        }
    }
    if(DEBUG) printf("Step 5b: Calculating free space left\n");
    //5b. Calculate Free space left
    int openBytesLeft = metadata->length - (int)length;
    int canPart = openBytesLeft > sizeof(Metadata);
    
    if(DEBUG) printf("Open bytes left: %d\n", openBytesLeft);
    if(DEBUG) printf("Can Partition: %d\n", canPart);
    if(DEBUG) printf("Step 5c: Patitioning if necessary\n");
    //5c + 5d. Partition if necessary
    if(canPart) {
        metadata->length = (int)length;
        ptr = incrementPointer(metadata, sizeof(Metadata) + metadata->length);
        ptr->status = OPEN;
        ptr->length = openBytesLeft - sizeof(Metadata);
    }
    if(DEBUG) printf("\tStep 5e: Returning address of size %d\n", (int) length);
    //5e. Return statement
    metadata->status = CLOSED;
    return incrementPointer(metadata, sizeof(Metadata));
}
/**
 * Function Description: myfree()
 *     1. Exit if given ptr is outside of the memory block.
 *     2. Exit if ptr - sizeof(Metadata) points to a valid metadata struct.
 *     3. Exit if valid metadata struct is closed.
 *     4. At this point metadata struct is open and valid, so we free it by setting status to OPEN and return.
 */
void myfree(void *ptr, char *file, int line) {
    if(DEBUG) printf("Bool1 : %d\t Bool2: %d\n", ptr < (void *) (memory + sizeof(Metadata)), ptr > (void *) (memory + MEM_SIZE));

    if(ptr < (void *) (memory + sizeof(Metadata)) || ptr > (void *) (memory + MEM_SIZE)) { // given ptr is outside of memory block
        printf("\tError: ptr is outside of memory block\n");
        return;
    }
    // jumps to where metadata should be given address of data
    Metadata *metadata = (Metadata *) (ptr - sizeof(Metadata));
    if((metadata->status != OPEN && metadata->status != CLOSED) || metadata->length < 1 || metadata->length > MEM_SIZE) { // check if this is a valid metadata ptr
        printf("\tError: Invalid ptr found\n");
        return;
    }

    // assuming valid metadata found, but could still be OPEN

    if(DEBUG) printf("Checking if metadata is closed, if not returning...\n");
    if(metadata->status == OPEN) {
        // metadata found but its open, so nothing to free
        printf("\tError: memory is already freed\n");
        return;
    }

    // assuming valid, closed metadata found

    metadata->status = OPEN;
    if(DEBUG) printf("Returning, metadata successfully marked as open\n");
    return;
}