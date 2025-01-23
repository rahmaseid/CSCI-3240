//Rahma Seid
//Rahma Seid
//CSCI 3240
//Project 4
//server.c: This program will take in the information passed in from the client.c and perform the actions and then write back to the client.

#define MAX_LENGTH 128
#define MAX_SIZE 1000
#define LINE_BUFFER_SIZE 10240  // Expanded line buffer to prevent overflow
#define MAX_RECORDS 1000
#define MAX_EMPLOYEES 1000 // Defined maximum number of employees
#define MAX_BUFFER_SIZE LINE_BUFFER_SIZE // Using LINE_BUFFER_SIZE for buffer size consistency
#include "csapp.h"
#include <stdio.h>
#include <string.h>
#include <pthread.h>

typedef struct {
    char firstName[MAX_LENGTH];
    char lastName[MAX_LENGTH];
    char zipCode[MAX_LENGTH];
    char department[MAX_LENGTH];
    int salary;
} Struct_Employee_Info;

Struct_Employee_Info employees[MAX_EMPLOYEES]; // Declared global employees array
int employee_count = 0; // Declared global employee count

//Reader-writer lock variables
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int reader_count = 0;
int writer_active = 0;

//Function Prototypes
void reload_employee_records(Struct_Employee_Info employees[], int *count);
void *severFunction(void *arg);
void enter_read_lock();
void exit_read_lock();
void enter_write_lock();
void exit_write_lock();


void reload_employee_records(Struct_Employee_Info employees[], int *count) {
    FILE *file_object = fopen("records.csv", "r");
    if (!file_object) {
        perror("Unable to open records.csv");
        return;
    }

    *count = 0;
    while (fscanf(file_object, "%[^,],%[^,],%[^,],%[^,],%d\n",
                  employees[*count].firstName,
                  employees[*count].lastName,
                  employees[*count].zipCode,
                  employees[*count].department,
                  &employees[*count].salary) == 5) {
        (*count)++;
        if (*count >= MAX_EMPLOYEES) break;  // Prevent overflow
    }
    fclose(file_object);
}

//Reader lock entry
void enter_read_lock() {
	pthread_mutex_lock(&mutex);
	while(writer_active > 0) {
		pthread_cond_wait(&cond, &mutex);
	}
	reader_count++;
	pthread_mutex_unlock(&mutex);
}

//Reader lock exit
void exit_read_lock(){
	pthread_mutex_lock(&mutex);
	reader_count--;
	if (reader_count == 0) {
		pthread_cond_broadcast(&cond);
	}
	pthread_mutex_unlock(&mutex);
}

//Writer lock entry
void enter_write_lock() {
	pthread_mutex_lock(&mutex);
	while(writer_active > 0 || reader_count > 0) {
		pthread_cond_wait(&cond, &mutex);
	}
	writer_active = 1;
	pthread_mutex_unlock(&mutex);
}

//Writer lock exit
void exit_write_lock() {
	pthread_mutex_lock(&mutex);
	writer_active = 0;
	pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&mutex);
}


void *serverFunction(void *arg) {
	int connfd = *((int *) arg);
	free(arg);
	pthread_detach(pthread_self());

    size_t n;
    char buffer[MAXLINE];
	char line[LINE_BUFFER_SIZE]; // Added for proper buffer management

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        n = read(connfd, buffer, sizeof(buffer));

        if (n <= 0) {
            perror("Error reading from client");
            break;
        }

        int act_option = atoi(buffer);
        memset(buffer, 0, sizeof(buffer));

        if (act_option == 1) {
            // Add new record (writer lock)
			enter_write_lock();
            char firstName[MAX_LENGTH], lastName[MAX_LENGTH];
            char zipCode[MAX_LENGTH], department[MAX_LENGTH];
            int salary;
			char success[] = "Ready";
            if (write(connfd, success, strlen(success) + 1) == -1) {
                perror("Error sending success message");
            }

            // Read fields from the client
            if ((n = read(connfd, firstName, sizeof(firstName))) <= 0) break;
            if ((n = read(connfd, lastName, sizeof(lastName))) <= 0) break;
            if ((n = read(connfd, zipCode, sizeof(zipCode))) <= 0) break;
            if ((n = read(connfd, department, sizeof(department))) <= 0) break;
            if ((n = read(connfd, buffer, sizeof(buffer))) <= 0) break;
            salary = atoi(buffer);

            // Append new record to the file
            FILE *file_object = fopen("records.csv", "a+");
            if (!file_object) {
                perror("Unable to open the file");
                exit_write_lock();
                break;
            }

            fprintf(file_object, "%s,%s,%s,%s,%d\n", firstName, lastName, zipCode, department, salary);
            fflush(file_object);
            fclose(file_object);
            exit_write_lock();

            char successMessage[] = "Record added Successfully!!\n";
            if (write(connfd, successMessage, strlen(successMessage) + 1) == -1) {
                perror("Error sending success message");
            }

        } else if (act_option == 2) {
            // Reload the records from CSV file each time a search is performed
			enter_read_lock();
            reload_employee_records(employees, &employee_count);
			char success[] = "Ready";
            if (write(connfd, success, strlen(success) + 1) == -1) {
                perror("Error sending success message");
            }

            char firstName[MAX_LENGTH], lastName[MAX_LENGTH];
            if ((n = read(connfd, firstName, sizeof(firstName))) <= 0) break;
            if ((n = read(connfd, lastName, sizeof(lastName))) <= 0) break;

            firstName[strcspn(firstName, "\n")] = '\0';
            lastName[strcspn(lastName, "\n")] = '\0';

            int found = 0;
            memset(line, 0, sizeof(line));
            for (int i = 0; i < employee_count; i++) {
                if (strcasecmp(employees[i].firstName, firstName) == 0 &&
                    strcasecmp(employees[i].lastName, lastName) == 0) {
                    char temp[MAXLINE];
                    snprintf(temp, sizeof(temp), "%.127s,%.127s,%.127s,%.127s,%d\n",
                             employees[i].firstName, employees[i].lastName,
                             employees[i].zipCode, employees[i].department,
                             employees[i].salary);
                    strncat(line, temp, sizeof(line) - strlen(line) - 1);
                    found = 1;
                }
            }
            if (!found) {
				char noRecord[] = "No record found!!\n";
                if (write(connfd, noRecord, strlen(noRecord) + 1) == -1) {
                    perror("Error sending no record found message");
                }
            } else {
                if (write(connfd, line, strlen(line) + 1) == -1) {
                    perror("Error sending found records");
                }
            }
            exit_read_lock();
        }
         else if (act_option == 3) {
            // Search by zip code
			enter_read_lock();
            reload_employee_records(employees, &employee_count);
            char zipCode[MAX_LENGTH];
			char success[] = "Ready";
            if (write(connfd, success, strlen(success) + 1) == -1) {
                perror("Error sending success message");
            }
			 
            if ((n = read(connfd, zipCode, sizeof(zipCode))) <= 0) {
		        perror("Error reading zip code from client");
		        exit_read_lock();
		        break;
		    }

            zipCode[strcspn(zipCode, "\n")] = '\0';

            int found = 0;
            memset(line, 0, sizeof(line));
            for (int i = 0; i < employee_count; i++) {
                if (strcmp(employees[i].zipCode, zipCode) == 0) {
                    char temp[MAXLINE];
                    snprintf(temp, sizeof(temp), "%.127s,%.127s,%.127s,%.127s,%d\n",
                             employees[i].firstName, employees[i].lastName,
                             employees[i].zipCode, employees[i].department,
                             employees[i].salary);
                    strncat(line, temp, sizeof(line) - strlen(line) - 1);
                    found = 1;
                }
            }
            if (!found) {
                char noRecord[] = "No record found!!\n";
                if (write(connfd, noRecord, strlen(noRecord) + 1) == -1) {
                    perror("Error sending no record found message");
                }
            } else {
                if (write(connfd, line, strlen(line) + 1) == -1) {
                    perror("Error sending found records");
                }
            }
            exit_read_lock();

        } else if (act_option == 4) {
            // Search by salary with comparison
			enter_read_lock();
            reload_employee_records(employees, &employee_count);

            char comparison[MAX_LENGTH];
            int salary;
			char success[] = "Ready";
            if (write(connfd, success, strlen(success) + 1) == -1) {
                perror("Error sending success message");
            }

            // Read salary value
		    if ((n = read(connfd, buffer, sizeof(buffer))) <= 0) {
		        perror("Error reading salary from client");
		        exit_read_lock();
		        break;
		    }
            salary = atoi(buffer);

            // Read comparison operator
		    if ((n = read(connfd, comparison, sizeof(comparison))) <= 0) {
		        perror("Error reading comparison operator from client");
		        exit_read_lock();
		        break;
		    }

            comparison[strcspn(comparison, "\n")] = '\0';

            int found = 0;
            memset(line, 0, sizeof(line));
            for (int i = 0; i < employee_count; i++) {
                int include = 0;
                if ((strcmp(comparison, ">=") == 0 && employees[i].salary >= salary) ||
                    (strcmp(comparison, "<=") == 0 && employees[i].salary <= salary) ||
                    (strcmp(comparison, "==") == 0 && employees[i].salary == salary) ||
                    (strcmp(comparison, ">") == 0 && employees[i].salary > salary) ||
                    (strcmp(comparison, "<") == 0 && employees[i].salary < salary)) {
                    include = 1;
                }

                if (include) {
                    char temp[MAXLINE];
                    snprintf(temp, sizeof(temp), "%.127s,%.127s,%.127s,%.127s,%d\n",
                             employees[i].firstName, employees[i].lastName,
                             employees[i].zipCode, employees[i].department,
                             employees[i].salary);
                    strncat(line, temp, sizeof(line) - strlen(line) - 1);
                    found = 1;
                }
            }

            if (!found) {
                char noRecord[] = "No record found!!\n";
                if (write(connfd, noRecord, strlen(noRecord) + 1) == -1) {
                    perror("Error sending no record found message");
                }
            } else {
                if (write(connfd, line, strlen(line) + 1) == -1) {
                    perror("Error sending found records");
                }
            }
            exit_read_lock();

        } else if (act_option == 5) {
            // Exit command received from client
            break;
        }
    }
    close(connfd);
	return NULL;
}

int main(int argc, char *argv[]) {
    int listenfd;
	pthread_t tid;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    listenfd = Open_listenfd(argv[1]);

    while (1) {
        struct sockaddr_storage clientaddr;
        socklen_t clientlen = sizeof(struct sockaddr_storage);
        int *connfdp = malloc(sizeof(int));
        *connfdp = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Pthread_create(&tid, NULL, serverFunction, connfdp);
    }
    return 0;
}
