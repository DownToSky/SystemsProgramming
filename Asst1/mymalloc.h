#include <stdio.h>


#define malloc( x ) mymalloc(x,__FILE__,__LINE__)

#define bool int
#define true 1
#define false 0

#define TOTAL_MEMORY_SIZE 5000

// struct representing a memory node in the memory chunk
typedef struct{
    bool is_occupid;
    size_t size;
}Node;


static char memory[TOTAL_MEMORY_SIZE];

static size_t size_left = 0;

static int nodes_used = 0;


