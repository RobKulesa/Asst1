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
    }
    // while iterating through free spaces
    // if length is small enough & metadata says space is free then malloc space for it and mark as closed
    // While loop stops for following 2 conditions: 1) We found free block big enough for metadata; 2) We reach the end of memory without finding a good block
    Metadata *ptr = memory;
    while((ptr->status == CLOSED || (ptr->status == OPEN && ptr->length < (int)length)) && ptr < memory+4096){
        ptr = ptr + sizeof(Metadata) + ptr->length;
    }
    if(ptr > memory + 4096){
        //error because space isn't big enough
    }
    //If we made it this far, we are fine
    return NULL;
}

void myfree(void *ptr, char *file, int line) {

}
