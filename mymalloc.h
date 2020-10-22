#define MEM_SIZE 4096
#define CLOSED 0
#define OPEN 1
#define DEBUG 0
#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)
typedef struct Metadata {
    int status;
    int length;
} Metadata;

void *mymalloc(size_t length, char *file, int line);
void myfree(void *ptr, char *file, int line);
Metadata *incrementPointer(Metadata *ptr, int n);