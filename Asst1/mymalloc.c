#include "mymalloc.h"

/*
 * memory needs to be initialized first time it is used by malloc
 * a new initialized memory is a single node that marks
 * the entirty of the rest of the bloc as accesible
 */
void initialize()
{
	size_used = sizeof(Node);
	nodes_used = 1;
	
	int pos = 0;
	for(; pos < TOTAL_MEMORY_SIZE; pos++)
		memory[pos] = 0;
	
	Node* first_node = (Node*)(memory);
	first_node -> size = TOTAL_MEMORY_SIZE - size_used;
	first_node -> is_occupied = false;
}


char* find_free_node(size_t size)
{
	char* ptr = memory;
	
	// traverse through all nodes and find one that is
	// not occupied and return it
	int i =0;
	for(; i < nodes_used; i++)
	{
		Node* curr_node = (Node*)ptr;
		if (curr_node -> size >=size && curr_node -> is_occupied == false)
			return ptr;
		
		// check next node in the next iteration
		ptr += sizeof(Node) + curr_node -> size;
	}
	return NULL;
}

char* allocate_node(char* addr, size_t size)
{
    Node* node = (Node*)addr;
    size_t prevSize = node->size;
    char* ret = addr + sizeof(Node);
    if (prevSize - size - sizeof(Node) >= 0)
    {
        size_t prevSize = node->size;
	    node->size = size;
	    node->is_occupied = true;
        Node* newNode = (Node*)(ret + size);
	    newNode->size = prevSize - size - sizeof(Node);
	    newNode->is_occupied = false;
	    size_used += size + sizeof(Node);
        nodes_used +=1;
    }
    else
    {
        node->is_occupied = true;
        size_used += size;
    }
	return ret;
}

void delete_node(char* addr)
{
	if(nodes_used == 1)
		return;
	
	Node* curr_node = (Node*)addr;
	Node* next_node = (Node*)(addr + curr_node -> size + sizeof(Node));
	
	if (curr_node -> is_occupied == false &&
		next_node -> is_occupied == false &&
		next_node -> size > 0)
		{
			nodes_used -= 1;
            size_used -= sizeof(Node);
			curr_node -> size += sizeof(Node) + next_node->size;
			delete_node(addr);
		}
}

void* mymalloc(size_t size, char* file, size_t line)
{
    if(nodes_used == 0)
        initialize();
    int space_needed = TOTAL_MEMORY_SIZE - size_used - size - sizeof(Node);
    // return null if asked for more memory than it has
    if(space_needed < 0)
        return NULL;
    char* ptr  = find_free_node(size);
	if(ptr == NULL)
		return NULL;
	ptr = allocate_node(ptr, size);
    return (void*) ptr;
}

void myfree(void* addr, char* file, size_t line)
{

    if(addr == NULL)
        return;
	
    bool found = false;

    char* ptr = memory;
	
	int i = 0;
	for(; i<nodes_used; i++)
	{
		// if found the node representing the given address
		Node* node = (Node*) ptr;
		char* metadata_pos = ptr + sizeof(Node);
		if (metadata_pos == addr)
		{
            found = true;
            if(node->is_occupied == false)
            {
                printf("ERROR: file:%s line:%d redundant freeing of the same pointer twice",file,line);
                return;
            }
			node -> is_occupied = false;
            size_used -= node->size;
			break;
		}
		ptr += node->size + sizeof(Node);
	}

    if(!found)
    {
        printf("ERROR: file:%s line:%d attempt to free a memory not allocated by malloc",file,line);
        return;
    }

    ptr = memory;
    i = 0;
    for(; i<nodes_used; i++)
    {
        Node* node = (Node*) ptr;
        if(node -> is_occupied == false)
        {
            delete_node(ptr);
        }
        ptr += node->size + sizeof(Node);
    }

}
