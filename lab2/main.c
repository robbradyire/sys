/*

	A program to simulate a producer-consumer pipeline using multiple
	threads and condition variables.

	Author: Robert Brady

*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define false 0
#define true 1
#define pipe_size 10
#define n_producers 1
#define n_consumers 1

int is_full = false;
int is_empty = true;
int num_pipe_items = 0;
int total_items = 20;
int items_per_thread;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty_cond = PTHREAD_COND_INITIALIZER;


void process_item() {
	struct timespec total, remaining;
	total.tv_sec = 0;
	total.tv_nsec = rand()/10;
	int i = 0;
	int err = true;
	while (err) {
		err = nanosleep(&total, &remaining);
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
	pthread_mutex_lock(&lock);
	while (total_items) {
		while (is_empty && total_items) {
			pthread_cond_wait(&empty_cond, &lock);
		}
		num_pipe_items--;
		total_items--;
		is_full = false;
		if (num_pipe_items == 0) {
			is_empty = true;
		}
		print_buffer(num_pipe_items);
		pthread_mutex_unlock(&lock);
		pthread_cond_broadcast(&full_cond);
		pthread_cond_broadcast(&empty_cond);
		process_item();
	}
	pthread_exit(NULL);
}

void * produce(void * arg) {
	int items_to_make = items_per_thread;

	pthread_mutex_lock(&lock);
	while (items_to_make--) {
		while (is_full && items_to_make) {
			pthread_cond_wait(&full_cond, &lock);
		}
		num_pipe_items++;
		is_empty = false;
		if (num_pipe_items == pipe_size) {
			is_full = true;
		}
		print_buffer(num_pipe_items);
		pthread_mutex_unlock(&lock);
		pthread_cond_broadcast(&empty_cond);
		pthread_cond_broadcast(&full_cond);
		process_item();
	}
	pthread_exit(NULL);
}

int main() {
	pthread_t producers[n_producers];
	pthread_t consumers[n_consumers];
	int i;
	items_per_thread = total_items / n_producers;

	for (i = 0; i < n_producers; i++) {
		pthread_create(&(producers[i]), NULL, produce, NULL);
	}

	for (i = 0; i < n_consumers; i++) {
		pthread_create(&(consumers[i]), NULL, consume, NULL);
	}

	for (i = 0; i < n_producers; i++) {
		pthread_join(producers[i], NULL);
	}

	pthread_cond_broadcast(&empty_cond);
	for (i = 0; i < n_consumers; i++) {
		pthread_join(consumers[i], NULL);
	}

	printf("All done.\n");
	return 0;
}
