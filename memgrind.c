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
    long long int timeA = 0, timeB = 0, timeC = 0;// timeD = 0, timeE = 0;
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
    }
    printf("Mean Time to execute workload A: %f microseconds\n", (timeA/CYCLES));
    printf("Mean Time to execute workload B: %f microseconds\n", (timeB/CYCLES));
    printf("Mean Time to execute workload C: %f microseconds\n", (timeC/CYCLES));
    //printf("Mean Time to execute workload D: %f microseconds\n", (timeD/CYCLES));
    //printf("Mean Time to execute workload E: %f microseconds\n", (timeE/CYCLES));
    return 0;
}