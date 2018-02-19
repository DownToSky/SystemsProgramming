
#include <stdlib.h>
#include "mymalloc.h"
#include <time.h>
#include <stdio.h>
#include <sys/time.h>

int main(int argc, const char* argv[])
{
    srand(time(NULL));
    int k =0;
    struct timeval t1,t2;
    double elapsedTime;
    gettimeofday(&t1,NULL);
    for(;k<100;k++){
        
        //Test1
        int i=0;
        for(; i<150; i++){
            free(malloc(1));
        }

        //Test2
        char* test2A[150];
        i=0;
        for(; i<150; i++){
            test2A[i] = malloc(1);
        }
        i=0;
        for(; i<150; i++){
            free(test2A[i]);
        }

        //Test3
        char* test3A[150];
        i = 0;
        int counter = 0;
        for(;counter<150;counter++)
        {
            if(i == 0)
            {
                test3A[0] = malloc(1);
                i++;
            }
            else
            {
                if(rand()%2 == 0)
                {
                    test3A[i] = malloc(1);
                    i++;
                }
                else
                {
                    i--;
                    free(test3A[i]);
                }
            }
        }


        //Test4
        char* test4A[150];
        i = 0;
        counter = 0;
        for(;counter<150;counter++)
        {
            if(i == 0)
            {
                test4A[0] = malloc(rand()%64+1);
                i++;
            }
            else
            {
                if(rand()%2 == 0)
                {
                    test4A[i] = malloc(rand()%64+1);
                    i++;
                }
                else
                {
                    i--;
                    free(test4A[i]);
                }
            }
        }



        //Test5
        char* test5A[150];
        i = 0;
        for(; i<150;i++){
            test5A[i] = malloc(1);
        }
        i=0;
        for(;i<150;i=i+2){
            free(test5A[i]);
        }
        i=1;
        for(;i<150;i=i+2){
            free(test5A[i]);
        }

        //Test6
        char* test6A[300];
        i=0;
        for(; i<300; i++){
            test6A[i] = malloc(1);
        }
        i=0;
        for(; i<300; i++){
            free(test6A[i]);
        }
    }
    gettimeofday(&t2, NULL);
    printf("elapsed time after 100 repetitions: %lfms\n",(t2.tv_usec-t1.tv_usec)/1000.0);
    return 0;


}
