#include <iostream>  // cout
#include <unistd.h>  // sleep
#include <pthread.h> // threads
#include <cstdio>

void TestBlock(void);

void *BlockInput(void *param);
void *BlockOutput(void *param);


int main()
{
	TestBlock();
	
	printf("\n");
	return 0;
}



void TestBlock()
{
	pthread_t t1, t2, t3;
	
	pthread_create(&t1, NULL, BlockInput, NULL);
	sleep(1);
	
	pthread_create(&t2, NULL, BlockOutput, NULL);
	sleep(1);
	
	pthread_create(&t3, NULL, BlockOutput, NULL);
	sleep(1);
	
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
}


void *BlockInput(void *param)
{
    char buf[32] = "";
    scanf("%s", buf);
    
    (void)param;
    return NULL;
}


void *BlockOutput(void *param)
{
    std::cout << "printing..." << std::endl;
    
    (void)param;
    return NULL;
}



