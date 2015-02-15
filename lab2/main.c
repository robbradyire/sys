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
		pthread_mutex_lock(&lock);
		while (is_empty) {
			pthread_cond_wait(&cond, &lock);
		}
		num_pipe_items--;
		is_full = false;
		if (num_pipe_items == 0) {
			is_empty = true;
		}
		pthread_mutex_unlock(&lock);
		pthread_cond_signal(&cond);
		process_item();
		printf("Item consumed. %d items in pipe.\n", num_pipe_items);
	}
}

void * produce(void * arg) {
	while (true) {
		pthread_mutex_lock(&lock);
		while (is_full) {
			pthread_cond_wait(&cond, &lock);
		}
		num_pipe_items++;
		is_empty = false;
		if (num_pipe_items == pipe_size) {
			is_full = true;
		}
		pthread_mutex_unlock(&lock);
		pthread_cond_signal(&cond);
		process_item();
		printf("Item produced. %d items in pipe.\n", num_pipe_items);
	}
}

int main() {
	int n_threads = 2;
	pthread_t producers[n_threads];
	pthread_t consumers[n_threads];
	int i, err1, err2;

	for (i = 0; i < n_threads; i++) {
		err2 = pthread_create(&(consumers[i]), NULL, consume, NULL);
		err1 = pthread_create(&(producers[i]), NULL, produce, NULL);
		if ((err1 | err2) != 0) {
			printf("Thread initialisation failed.\n");
			return 1;
		}
	}

	for (i = 0; i < n_threads; i++) {
		pthread_join(&(consumers[i]), NULL);
		pthread_join(&(producers[i]), NULL);
	}
}







