#include <stdio.h>


#define malloc( x ) mymalloc(x,__FILE__,__LINE__)

#define bool int
#define true 1
#define false 0

#define TOTAL_MEMORY_SIZE 5000

// struct representing a memory node in the memory chunk
typedef struct{
    bool is_occupied;
    size_t size;
}Node;

// on the machine tested, char is one byte
static char memory[TOTAL_MEMORY_SIZE];

static size_t size_used;

static int nodes_used;

void initialize();

char* find_free_node(char* beginning, size_t size);

char* allocate_node(char* free_node, size_t size);

void delete_node(char* addr);

void test();

void* mymalloc(size_t size, char* file, size_t line);

void myfree(void* addr, char* file, size_t line);