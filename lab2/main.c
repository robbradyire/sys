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
int is_empty = false;
int num_pipe_items = 2;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty_cond = PTHREAD_COND_INITIALIZER;


void process_item() {
	int jobsize = rand()/2;
	int i = 0;
	while (i < jobsize) {
		i++;
	}
}


void print_buffer(int num_pipe_items) {
	int i;
	printf("|");
	for (i = 0; i < num_pipe_items; i++) {
		printf("=|");
	}
	printf("\n");
}


void * consume(void * arg) {
	int rc;

	pthread_mutex_lock(&lock);
	while (true) {
		while (is_empty) {
			rc = pthread_cond_wait(&empty_cond, &lock);
			if (rc) {
				pthread_mutex_unlock(&lock);
			}
		}
		num_pipe_items--;
		is_full = false;
		if (num_pipe_items == 0) {
			is_empty = true;
		}
		print_buffer(num_pipe_items);
		pthread_mutex_unlock(&lock);
		pthread_cond_signal(&full_cond);
		process_item();
	}
}

void * produce(void * arg) {
	int rc;

	pthread_mutex_lock(&lock);
	while (true) {
		while (is_full) {
			rc = pthread_cond_wait(&full_cond, &lock);
			if (rc) {
				pthread_mutex_unlock(&lock);
			}
		}
		num_pipe_items++;
		is_empty = false;
		if (num_pipe_items == pipe_size) {
			is_full = true;
		}
		print_buffer(num_pipe_items);
		pthread_mutex_unlock(&lock);
		pthread_cond_signal(&empty_cond);
		process_item();
	}
}

int main() {
	int n_threads = 5;
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