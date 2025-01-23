//Rahma Seid
//CSCI 3240
//Lab 11 Part A: Write a program that calculates various statistical values for a list of numbers using POSIX threads.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//Structure to store data for threads
typedef struct {
	int *numbers;
	int count;
	double average;
	int minimum;
	int maximum;
} StatsData;

//Thread function to calculate the average
void *calc_average(void *arg) {
	StatsData *data = (StatsData *)arg;
	int sum = 0;
	for (int i = 0; i < data->count; i++) {
		sum += data->numbers[i];
	}
	data->average = (double)sum / data->count;
	return NULL;
}

//Thread function to calculate minimum
void *calc_minimum(void *arg) {
	StatsData *data = (StatsData *)arg;
	int min = data->numbers[0];
	for (int i = 1; i <data->count; i++) {
		if (data->numbers[i] < min) {
			min = data->numbers[i];
		}
	}
	data->minimum = min;
	return NULL;
}

//Thread function to calculate maximum
void *calc_maximum(void *arg) {
	StatsData *data = (StatsData *)arg;
	int max = data->numbers[0];
	for (int i = 1; i < data->count; i++) {
		if (data->numbers[i] > max) {
			max = data->numbers[i];
		}
	}
	data->maximum = max;
	return NULL;
}

int main(int argc, char *argv[]) {
	if (argc < 2){
		printf("Usage: %s <list of numbers>\n", argv[0]);
		return 1;
	}
	int count = argc - 1;
	int numbers[count];
	for (int i = 0; i < count; i++) {
		numbers[i] = atoi(argv[i+1]);
	}
	StatsData data;
	data.numbers = numbers;
	data.count = count;

	//Thread IDs
	pthread_t avg_thread, min_thread, max_thread;

	//Create threads for each calculation
	pthread_create(&avg_thread, NULL, calc_average, &data);
	pthread_create(&min_thread, NULL, calc_minimum, &data);
	pthread_create(&max_thread, NULL, calc_maximum, &data);

	//Wait for threads to complete
	pthread_join(avg_thread, NULL);
	pthread_join(min_thread, NULL);
	pthread_join(max_thread, NULL);

	//Display the results
	printf("The average value is %.2f\n", data.average);
	printf("The minimum value is %d\n", data.minimum);
	printf("The maximum value is %d\n", data.maximum);
}