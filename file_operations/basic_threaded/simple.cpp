#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *printHello(void *arg){
	int tid;
	tid = (long)arg;
	printf("Hello World %d\n", tid);
}

int main(){
	#define NTHREADS 20

	long threadid;
	pthread_t thread[NTHREADS];
	for(threadid = 0; threadid < NTHREADS; threadid++){
		pthread_create(&thread[threadid], NULL, printHello, (void *)threadid);	
	}

	pthread_exit(NULL);
}