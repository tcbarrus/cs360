#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <iostream>
#include <semaphore.h>

sem_t full, empty, mutex;

class myqueue {
	std::queue <int> stlqueue;
	public:
	void push (int sock){
		sem_wait(&empty);
		sem_wait(&mutex);
		stlqueue.push(sock);
		sem_post(&mutex);
		sem_post(&full);
	}

	int pop(){
		sem_wait(&full);
		sem_wait(&mutex);
		int rval = stlqueue.front();
		stlqueue.pop();
		sem_post(&mutex);
		sem_post(&empty);
		return rval;
	}
} sockqueue;

void *howdy(void *arg){
	while(1){
		sockqueue.pop();
		int tid; 
		tid = (long)arg;
		std::cout << "Handled from thread " << tid << std::endl;
		printf("HI %d\n", tid);
	}
}

int main(){
#define NTHREADS 10
#define NQUEUE 20
	
	long threadid;
	pthread_t threads[NTHREADS];
	sem_init(&full, PTHREAD_PROCESS_PRIVATE, 0);
	sem_init(&empty, PTHREAD_PROCESS_PRIVATE, NQUEUE);
	sem_init(&mutex, PTHREAD_PROCESS_PRIVATE, 1);

	for(threadid = 0; threadid < NTHREADS; threadid++){
		std::cout << "CREATE" << std::endl;
		pthread_create(&thread[threadid], NULL, howdy, (void *)threadid);
	}

	for(int i = 0; i < 10; i++){
		sockqueue.push(i);
	}
	
	while(true){}
	// for(int i = 0; i < 10; i++){
	// 	std::cout<<"Got "<<sockqueue.pop()<<std::endl;
	// }
}
