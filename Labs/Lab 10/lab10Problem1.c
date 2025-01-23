//Rahma Seid
//CSCI 3240
//Lab 10: Write a concurrent program that calculates various statistical values for a list of numbers using fork system call

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Usage: %s <num1> <num2> ... <numN>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	//Convert command line arguments to integers
	int numbers[argc - 1];
	for (int i = 1; i < argc; i++){
		numbers[i - 1] = atoi(argv[i]);
	}
	int num_count = argc - 1;

	pid_t pid;
	int status;

	//Create a process to calculate the average
	pid = fork();
	if (pid < 0) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0){
		//Child process for average
		double sum = 0;
		for (int i = 0; i < num_count; i++) {
			sum += numbers[i];
		}
		double average = sum / num_count;
		printf("The average value is %.2f\n", average);
		exit(EXIT_SUCCESS);
	}

	//Create a process to find the minimum value
	pid = fork();
	if (pid < 0) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0) {
		//Child process for minimum
		int min = numbers[0];
		for (int i = 1; i < num_count; i++) {
			if (numbers[i] < min) {
				min = numbers[i];
			}
		}

		printf("The minimum value is %d\n", min);
		exit(EXIT_SUCCESS);
	}

	
	//Create a process to find the maximum value
	pid = fork();
	if (pid < 0) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0){
		//Child process for maximum
		int max = numbers[0];
		for (int i = 1; i < num_count; i++){
			if (numbers[i] > max) {
				max = numbers[i];
			}
		}
		printf("The maximum value is %d\n", max);
		exit(EXIT_SUCCESS);
	}

	//Parent process waits for all chuld processes to finish
	for (int i = 0; i < 3; i++) {
		wait(&status);
	}

	return 0;
}





