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
#define true !false
#define jobsize 1000000000
#define pipe_size 10

int is_full = false;
int is_empty = true;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

/*
	a method that "processes" an item from the pipeline
*/
void process_item() {
	int i = 0;
	while (i < jobsize) {
		i++;
	}
}

int main() {
	process_item();
	return 0;
}