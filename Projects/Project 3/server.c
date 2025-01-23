//Rahma Seid
//CSCI 3240
//Project 3
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

typedef struct {
    char firstName[MAX_LENGTH];
    char lastName[MAX_LENGTH];
    char zipCode[MAX_LENGTH];
    char department[MAX_LENGTH];
    int salary;
} Struct_Employee_Info;

Struct_Employee_Info employees[MAX_EMPLOYEES]; // Declared global employees array
int employee_count = 0; // Declared global employee count

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

void serverFunction(int connfd, char client_hostname[MAXLINE], char client_port[MAXLINE]) {
    FILE *file_object;
    char line[LINE_BUFFER_SIZE];  // Increased buffer size for line
    size_t n;

    reload_employee_records(employees, &employee_count);

    char buffer[MAXLINE];

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
            // Add new record
            char firstName[MAX_LENGTH], lastName[MAX_LENGTH];
            char zipCode[MAX_LENGTH], department[MAX_LENGTH];
            int salary;

            if (employee_count >= MAX_EMPLOYEES) {
                char errorMessage[] = "Employee limit reached. Cannot add more records.\n";
                if (write(connfd, errorMessage, strlen(errorMessage) + 1) == -1) {
                    perror("Error sending employee limit message");
                }
                continue;
            }

            // Read fields from the client
            if ((n = read(connfd, firstName, sizeof(firstName))) <= 0) break;
            if ((n = read(connfd, lastName, sizeof(lastName))) <= 0) break;
            if ((n = read(connfd, zipCode, sizeof(zipCode))) <= 0) break;
            if ((n = read(connfd, department, sizeof(department))) <= 0) break;
            if ((n = read(connfd, buffer, sizeof(buffer))) <= 0) break;
            salary = atoi(buffer);

            // Append new record to the file
            file_object = fopen("records.csv", "a+");
            if (!file_object) {
                perror("Unable to open the file");
                break;
            }

            fprintf(file_object, "%s,%s,%s,%s,%d\n", firstName, lastName, zipCode, department, salary);
            fflush(file_object);  // Ensure the data is written
            fclose(file_object);

            // Add to in-memory employee array
            strcpy(employees[employee_count].firstName, firstName);
            strcpy(employees[employee_count].lastName, lastName);
            strcpy(employees[employee_count].zipCode, zipCode);
            strcpy(employees[employee_count].department, department);
            employees[employee_count].salary = salary;
            employee_count++;

            char successMessage[] = "Record added Successfully!!\n";
            if (write(connfd, successMessage, strlen(successMessage) + 1) == -1) {
                perror("Error sending success message");
            }

        } else if (act_option == 2) {
            // Reload the records from CSV file each time a search is performed
            reload_employee_records(employees, &employee_count);
        
            char firstName[MAX_LENGTH], lastName[MAX_LENGTH];
            if ((n = read(connfd, firstName, sizeof(firstName))) <= 0) break;
            if ((n = read(connfd, lastName, sizeof(lastName))) <= 0) break;
        
            firstName[strcspn(firstName, "\n")] = '\0';  // Remove trailing newline
            lastName[strcspn(lastName, "\n")] = '\0';    // Remove trailing newline
        
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
        }
         else if (act_option == 3) {
            // Search by zip code
            reload_employee_records(employees, &employee_count);
            char zipCode[MAX_LENGTH];
            if ((n = read(connfd, zipCode, sizeof(zipCode))) <= 0) break;

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

        } else if (act_option == 4) {
            // Search by salary with comparison
            reload_employee_records(employees, &employee_count);
            char comparison[MAXLINE];
            int salary;

            // Read salary value and comparison operator
            if ((n = read(connfd, buffer, sizeof(buffer))) <= 0) break;
            salary = atoi(buffer);

            if ((n = read(connfd, comparison, sizeof(comparison))) <= 0) break;

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

        } else if (act_option == 5) {
            // Exit command received from client
            break;
        }
    }
    close(connfd);
}

int main(int argc, char *argv[]) {
    int listenfd;
    int connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    char client_hostname[MAXLINE], client_port[MAXLINE];

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    listenfd = Open_listenfd(argv[1]);

    while (1) {
        clientlen = sizeof(struct sockaddr_storage);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *)&clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);
        printf("Connected to (%s, %s)\n", client_hostname, client_port);
        serverFunction(connfd, client_hostname, client_port);
    }
    exit(0);
}
