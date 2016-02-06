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
	int tid; 
	tid = (long)arg;
	printf("HI %d\n", tid);
}

int main(){
#define NTHREADS 10
#define NQUEUE 20
	
	long threadid;
	pthread_t threads[NTHREADS];
	sem_open(&full, PTHREAD_PROCESS_PRIVATE, 0);
	sem_open(&empty, PTHREAD_PROCESS_PRIVATE, NQUEUE);
	sem_open(&mutex, PTHREAD_PROCESS_PRIVATE, 1);

	for(int i = 0; i < 10; i++){
		sockqueue.push(i);
	}

	for(int i = 0; i < 10; i++){
		std::cout<<"Got "<<sockqueue.pop()<<std::endl;
	}
}
