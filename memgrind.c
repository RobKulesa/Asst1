#include <stdio.h>
#include <time.h>
#include "mymalloc.h"
/*
 * Runs workloads, calculates and output mean time for each workload
*/
int main(int argc, char** argv) {
    int i;
    for(i = 0; i < 50; i++){
        printf("ITERATION %d\n", i);
        char *ptr = malloc(4087);
        free(ptr);
    }
    
    return 0;
}