#include <stdlib.h>
#include "mymalloc.h"
#include <time.h>

int main(int argc, const char* argv[])
{
	char* arr[1000];
	int i=0;
	for(; i<1000; i++){
		arr[i] = malloc(1);		
	}
	
	i=0
	for(; i<1000; i++){
		free(arr[i]);
	}
	reutrn 0;
}