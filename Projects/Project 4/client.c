//Rahma Seid
//CSCI 3240
//Project 4
//client.c: This program runs a client and server based interactions where the server takes in a request from the user (using the read() function) and does the appropriate commands to the records.csv file.

#include "csapp.h"
#include <stdio.h>

void print_menu();

int main(int argc, char *argv[]) {
    int clientfd;
    char *host, *port;
    size_t n;

    char buffer[MAXLINE];
    char input[MAXLINE];  // Buffer for reading user input

    bzero(buffer, MAXLINE);

    if (argc != 3) {
        fprintf(stderr, "usage %s <host> <port>\n", argv[0]);
        exit(0);
    }

    host = argv[1];
    port = argv[2];

    clientfd = Open_clientfd(host, port);
    int user_choice;
    int w_result;

    while (1) {
        print_menu();
        if (fgets(input, sizeof(input), stdin) == NULL) {  // Read user input
            perror("Error reading input");
            exit(1);
        }
        w_result = sscanf(input, "%d", &user_choice);  // Parse user input
        if (w_result != 1) {
            printf("Invalid input!\n");
            continue;
        }

        if (user_choice == 1) {
            // Add Record
            printf("\n");
            char firstName[MAXLINE], lastName[MAXLINE], department[MAXLINE];
            char zipCode[MAXLINE], salary[MAXLINE];
            char choice[] = "1";

            // Send option to server
            n = write(clientfd, choice, strlen(choice) + 1);
            if (n < 0) {
                perror("Error sending choice to server");
                continue;
            }
			n = read(clientfd, buffer, MAXLINE);

            // Read and write First Name
            printf("Enter First Name: ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                perror("Error reading input");
                exit(1);
            }
            sscanf(input, "%127s", firstName);
            n = write(clientfd, firstName, strlen(firstName) + 1);

            // Read and write Last Name
            printf("Enter Last Name: ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                perror("Error reading input");
                exit(1);
            }
            sscanf(input, "%127s", lastName);
            n = write(clientfd, lastName, strlen(lastName) + 1);

            // Read and write Zip Code
            printf("Enter Zip Code: ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                perror("Error reading input");
                exit(1);
            }
            sscanf(input, "%127s", zipCode);
            n = write(clientfd, zipCode, strlen(zipCode) + 1);

            // Read and write Department
            printf("Enter Department: ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                perror("Error reading input");
                exit(1);
            }
            sscanf(input, "%127s", department);
            n = write(clientfd, department, strlen(department) + 1);

            // Read and write Salary
            printf("Enter Salary: ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                perror("Error reading input");
                exit(1);
            }
            sscanf(input, "%127s", salary);
            n = write(clientfd, salary, strlen(salary) + 1);

            // Receive response from server
            n = read(clientfd, buffer, MAXLINE);
            if (n > 0) {
                buffer[n] = '\0';
                printf("Message from the server: \n");
				printf("%s\n", buffer);
            } else {
                printf("No response from server.\n");
            }
            bzero(buffer, MAXLINE);
        }
        else if (user_choice == 2) {
            // Search by Name
            printf("\n");
            char firstName[MAXLINE], lastName[MAXLINE];
            char choice[] = "2";

            // Send choice
            n = write(clientfd, choice, strlen(choice) + 1);
			n = read(clientfd, buffer, MAXLINE);

            // Read and send First Name
            printf("Enter First Name: ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                perror("Error reading input");
                exit(1);
            }
            sscanf(input, "%127s", firstName);
            n = write(clientfd, firstName, strlen(firstName) + 1);

            // Read and send Last Name
            printf("Enter Last Name: ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                perror("Error reading input");
                exit(1);
            }
            sscanf(input, "%127s", lastName);
            n = write(clientfd, lastName, strlen(lastName) + 1);

            // Read server response
            n = read(clientfd, buffer, MAXLINE);
            if (n > 0) {
                buffer[n] = '\0';
                printf("Message from the server: \n");
				printf("%s\n", buffer);
            } else {
                printf("No response from server.\n");
            }
            bzero(buffer, MAXLINE);
        }
        else if (user_choice == 3) {
            // Search by Zip Code
            printf("\n");
            char zipCode[MAXLINE];
            char choice[] = "3";

            n = write(clientfd, choice, strlen(choice) + 1);
			n = read(clientfd, buffer, MAXLINE);

            // Read and write Zip Code
            printf("Enter Zip Code: ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                perror("Error reading input");
                exit(1);
            }
            sscanf(input, "%127s", zipCode);
            n = write(clientfd, zipCode, strlen(zipCode) + 1);

            // Read server response
            n = read(clientfd, buffer, MAXLINE);
            if (n > 0) {
                buffer[n] = '\0';
                printf("Message from the server: \n");
				printf("%s\n", buffer);
            } else {
                printf("No response from server.\n");
            }
            bzero(buffer, MAXLINE);
        }
        else if (user_choice == 4) {
            // Search by Salary
            printf("\n");
            char comparison[MAXLINE];
            char salary[MAXLINE];
            char choice[] = "4";

            n = write(clientfd, choice, strlen(choice) + 1);
			n = read(clientfd, buffer, MAXLINE);

            // Read and write Salary
            printf("Enter Salary: ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                perror("Error reading input");
                exit(1);
            }
            sscanf(input, "%127s", salary);
            n = write(clientfd, salary, strlen(salary) + 1);

            // Read and write Comparison Type
            printf("Enter Comparison Type [‘>’,‘<’,‘==’,‘>=’,‘<=’]: ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                perror("Error reading input");
                exit(1);
            }
            sscanf(input, "%127s", comparison);
            n = write(clientfd, comparison, strlen(comparison) + 1);

            // Receive server response
            n = read(clientfd, buffer, MAXLINE);
            if (n > 0) {
                buffer[n] = '\0';
                printf("Message from the server: \n");
				printf("%s\n", buffer);
            } else {
                printf("No response from server.\n");
            }
            bzero(buffer, MAXLINE);
        }
        else if (user_choice == 5) {
            // Terminate
            char choice[] = "5";
            n = write(clientfd, choice, strlen(choice) + 1);
			n = read(clientfd, buffer, MAXLINE);
            if (n < 0) {
                perror("Error sending termination command");
            }
			// Receive server response
            printf("Message from the server:\n");
			printf("Connection Closed!!\n");
            break;
        }
        else {
            printf("Invalid option. Please select between 1 and 5.\n");
        }
    }

    Close(clientfd);
    return 0;
}

void print_menu() {
    printf("(1) Add Record\n");
    printf("(2) Search by Name\n");
    printf("(3) Search by Zip Code\n");
    printf("(4) Search by Salary\n");
    printf("(5) Terminate\n");
    printf("Select an option [1,2,3,4, or 5]: ");
}
