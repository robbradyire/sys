/*

	A program to simulate a producer-consumer pipeline using multiple
	threads and condition variables.

	Author: Robert Brady

*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define false 0
#define true 1
#define pipe_size 10

int is_full = false;
int is_empty = true;
int num_pipe_items = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

/*
	a method that "processes" an item from the pipeline
*/
void process_item() {
	int jobsize = rand()/2;
	int i = 0;
	while (i < jobsize) {
		i++;
	}
}

void * consume(void * arg) {
	while (true) {
		while (is_empty == true) {
			pthread_cond_wait(&cond, &lock);
		}
		pthread_mutex_lock(&lock);
		num_pipe_items--;
		if (num_pipe_items == 0) {
			is_empty = true;
		}
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&lock);
		process_item();
	}
}

void * produce(void * arg) {
	while (true) {
		while (is_full) {
			pthread_cond_wait(&cond, &lock);
		}
		pthread_mutex_lock(&lock);
		num_pipe_items++;
		if (num_pipe_items == pipe_size) {
			is_full = true;
		}
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&lock);
	}
}

int main() {
	process_item();
	process_item();
	return 0;
}







