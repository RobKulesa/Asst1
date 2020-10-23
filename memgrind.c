#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include "mymalloc.h"
#define CYCLES 50.0
/*
 * Runs workloads, calculates and output mean time for each workload
*/
int main(int argc, char** argv) {
    int i, j;
    long long int timeA = 0, timeB = 0, timeC = 0, timeD = 0, timeE = 0;
    for(j = 0; j < (int) CYCLES; ++j) {
        struct timeval start;
        struct timeval end;
        //A.
        gettimeofday(&start, 0); 
        for(i = 0; i < 120; i++){
            char *ptr = malloc(1);
            free(ptr);
        }
        gettimeofday(&end, 0);
        timeA += (end.tv_sec - start.tv_sec)*pow(10, 6) + end.tv_usec - start.tv_usec;
        //printf("Task A time total in microseconds: %lld\n", timeA);
        //printf("Task A Complete\n");

        //B.
        gettimeofday(&start, NULL); 
        void* arr[120];
        for(i = 0; i < 120; i++) {
            arr[i] = (void*)malloc(1);
        }
        for(i = 0; i < 120; i++) {
            free(arr[i]);
        }
        gettimeofday(&end, NULL);
        timeB += (end.tv_sec - start.tv_sec)*pow(10, 6) + end.tv_usec - start.tv_usec;
        //printf("Task B Complete\n");

        //C.
        //0 = MALLOC
        //1 = FREE
        gettimeofday(&start, NULL);
        char* ptrArr[120] = {malloc(1)};
        int mallocsLeft = 1;
        int mallocCounter = 1;
        srand(time(NULL));
        for(i = 0; i < 239; ++i) {
            int random = rand() % 2;
            if(random == 0 && mallocCounter < 120) {
                ptrArr[mallocsLeft] = malloc(1);
                ++mallocCounter;
                ++mallocsLeft;
            } else if(mallocsLeft > 0) {
                --mallocsLeft;
                free(ptrArr[mallocsLeft]);
                ptrArr[mallocsLeft] = 0;
            } else {
                ptrArr[mallocsLeft] = malloc(1);
                ++mallocCounter;
                ++mallocsLeft;
            }
        }
        gettimeofday(&end, NULL);
        timeC += (end.tv_sec - start.tv_sec)*pow(10, 6) + end.tv_usec - start.tv_usec;
       //printf("Task C Complete\n");
        
        void *a;
        void *b;
        void *c;
        void *d;


        //D.
        // Workload description: Can malloc automatically merge free blocks as it finds them?
        gettimeofday(&start, NULL);
        for(int i = 0; i < 50; i++){
            //printf("Iteraation: %d\n", i);
            //Fill the array
                //Bytes used by data: 1016*4 = 4064
                //Bytes used by metadata: 32*4 = 32
                //Total Bytes: 4064 + 32 = 4096
            //printf("**Mallocing a\n");
            a = malloc(1016);
            //printf("**Mallocing a\n");
            b = malloc(1016);
            //printf("**Mallocing a\n");
            c = malloc(1016);
            //printf("**Mallocing a\n");
            d = malloc(1016);
            //Free a and b. This should leave us with 2032 + 16 = 2048 open bytes
            free(a);
            free(b);

            //Allocate a 2040 Size chunk
            //printf("**Mallocing FATTY1\n");
            a = malloc(2040);
            
            //Free c and d
            free(c);
            free(d);
            
            //Allocate another 2040 Chunk
            //printf("**Mallocing FATTY2\n");
            b = malloc(2040);
            
            //free 2040 chunks
            free(a);
            free(b);

            //malloc 4088 chunk
            a = malloc(4088);
            free(a);

        }
        gettimeofday(&end, NULL);
        timeD += (end.tv_sec - start.tv_sec)*pow(10, 6) + end.tv_usec - start.tv_usec;

        //E
        gettimeofday(&start, NULL);
        for(i = 0; i < 50; i++){
            a = malloc(2040);
            b = malloc(2040);
            free(a);
            free(b);
            a = malloc(2035);
            b = malloc(2045);
            free(a);
            free(b);
        }
        gettimeofday(&end, NULL);
        timeE += (end.tv_sec - start.tv_sec)*pow(10, 6) + end.tv_usec - start.tv_usec;
    }
    printf("Mean Time to execute workload A: %f microseconds\n", (timeA/CYCLES));
    printf("Mean Time to execute workload B: %f microseconds\n", (timeB/CYCLES));
    printf("Mean Time to execute workload C: %f microseconds\n", (timeC/CYCLES));
    printf("Mean Time to execute workload D: %f microseconds\n", (timeD/CYCLES));
    printf("Mean Time to execute workload E: %f microseconds\n", (timeE/CYCLES));
    //printf("Mean Time to execute workload D: %f microseconds\n", (timeD/CYCLES));
    //printf("Mean Time to execute workload E: %f microseconds\n", (timeE/CYCLES));
    return 0;
}