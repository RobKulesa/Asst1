#include <stdio.h>
#include <errno.h>
#include "mymalloc.h"

static char memory[MEM_SIZE];

void *mymalloc(size_t length, char *file, int line) {
    if(length == 0) return NULL;
    // return error if negative length is given or a length greater than MEM_SIZE
    Metadata *metadata = (Metadata *) memory;
    //Check to see if the memory array hasn't been initialized
    //This is because we should not have blocks of length 0
    if(metadata->length == 0) { //Memory is uninitialized
        metadata->length = MEM_SIZE - sizeof(Metadata);
        metadata->status = OPEN;
        memory[0] = metadata;
        // size of metadata + malloc'd length = start of next block
        // memory is empty
        printf("hi uh our memory is empty yo\n");
    } else {
        // memory has first metadata
        printf("found metadata at beginning of memory with length %d with status %d\n", test->length, test->status);
    }

    
    return NULL;
}

void myfree(void *ptr, char *file, int line) {

}
