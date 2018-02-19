#include "mymalloc.h"

/*
 * memory needs to be initialized before it is used by malloc
 * a new initialized memory is a single node that marks
 * the entirty of the rest of the bloc as accesible
 */
void initialize()
{
	size_used = sizeof(Node*);
	nodes_used = 1;
	
	int pos = 0;
	for(pos; pos < TOTAL_MEMORY_SIZE; pos++)
		memory[pos] = 0;
	
	Node* first_node = (Node*)(memory);
	first_node -> size = TOTAL_MEMORY_SIZE - size_used;
	first_node -> is_occupied = false;
}

void print_all_nodes()
{
	puts("The current statistics of dynamic memory usage:");
	printf("Allocated: %d\
	Left: %d\
    Nodes used: %d\n\n", size_used, TOTAL_MEMORY_SIZE-size_used,nodes_used);
    int i = 0;
	char* ptr = memory;
	for(i;i < TOTAL_MEMORY_SIZE; i++)
	{
		Node * node = (Node*)(ptr);
		//printf("Node number: %d\t\
		Node Address: %p\n\
		is occupied: %d\t\
		Node size: %d\n\
		___________________\n", i, ptr, node->is_occupied, node->size);
		ptr += node -> size + sizeof(Node*) + 1;
	}
}

char* find_free_node(char* beginning, size_t size)
{
	char* ptr = beginning;
	
	// traverse through all nodes and find one that is
	// not occupied and return it
	int i =0;
	for(; i < nodes_used; i++)
	{
		Node* curr_node = (Node*)ptr;
		if (curr_node -> size >=size && curr_node -> is_occupied == false)
			return ptr;
		
		// check next node in the next iteration
		ptr += sizeof(Node*) + curr_node -> size + 1;
	}
	return NULL;
}

char* allocate_node(char* open_position, size_t request_size)
{
	
	//store the size of the original block of memory
	size_t old_size = ((MemNode*)(open_position))->mem_size;
	
	//1. update the old memory size with the new request size	
	//2. update the dirty to show that there is data now written to this position
	((MemNode*)(open_position))->mem_size = request_size;
	((MemNode*)(open_position))->dirty_bit = true;
	
	//Pointer to the malloced position the user requested
	//The value that we are going to return.
	char* write_pos = open_position + sizeof(MemNode*) + 1;

	//create a new MemNode right after the data we are going to return to the user
	//TODO: Check to see if we need to add this +1
	char* new_mem_node = write_pos + request_size;
	((MemNode*)(new_mem_node))->mem_size = old_size - request_size - sizeof(MemNode*);
	((MemNode*)(new_mem_node))->dirty_bit = false;
	
	memory_count += request_size + sizeof(MemNode*);
	mem_node_count += 1;

	return write_pos;
}

void delete_node(char* addr)
{
	if(nodes_used == 1)
		return;
	
	Node* curr_node = (Node*)addr;
	Node* next_node = (Node*)(addr + curr_node -> size + sizeof(Node*) + 1);
	
	if (curr_node -> is_occupied == false &&
		next_node -> is_occupied == false &&
		next_node -> size > 0)
		{
			nodes_used -= 1;
			curr_node -> size += sizeof(Node*) + next_node->size;
			delete_node(addr);
		}
}

void* mymalloc(size_t size, char* file, size_t line)
{
	if(size > TOTAL_MEMORY_SIZE - size_used)
		return NULL;
    char* ptr  = find_free_node(memory, size);
	if(ptr == NULL)
		return NULL;
	ptr = allocate_node(ptr, size);
	return (void*) ptr;
}

void myfree(void* addr, char* file, size_t line)
{
	char* ptr = memory;
	
	int i = 0;
	for(; i<nodes_used; i++)
	{
		// if found the node representing the given address
		Node* node = (Node*) addr;
		char* metadata_pos = ptr + sizeof(Node*) + 1;
		if (metadata_pos == addr)
		{
			node -> is_occupied = false;
			delete_node(ptr);
			break;
		}
		ptr += node->size + sizeof(Node*) + 1;
	}
}
