//Rahma Seid
//CSCI 3240
//Lab 6
//Task: Write a program that reads the employee details from records.csv and stores it into a structure.

//Write your code in between TODO start and TODO end sections
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 1000
#define MAX_LENGTH 25

typedef struct {
    char firstName[MAX_LENGTH];
    char lastName[MAX_LENGTH];
    char zipCode[MAX_LENGTH];
    char department[MAX_LENGTH];
    int salary;
} Struct_Employee_Info;

// Search by Name
char searchByName(Struct_Employee_Info employees[], int count, const char *firstName, const char *lastName) {
    for (int i = 0; i < count; i++) {
        if (strcmp(employees[i].firstName, firstName) == 0 && strcmp(employees[i].lastName, lastName) == 0) {
            printf("Name: %s %s\tZip Code: %s\tDepartment: %s\tSalary: %d\n",
                   employees[i].firstName, employees[i].lastName, employees[i].zipCode, employees[i].department, employees[i].salary);
            return 'Y';  // Found
        }
    }
    return 'N';  // Not found
}

// Search by Zip Code
char searchByZipCode(Struct_Employee_Info employees[], int count, const char *zipCode) {
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(employees[i].zipCode, zipCode) == 0) {
            printf("Name: %s %s\tZip Code: %s\tDepartment: %s\tSalary: %d\n",
                   employees[i].firstName, employees[i].lastName, employees[i].zipCode, employees[i].department, employees[i].salary);
            found = 1;
        }
    }
    return found ? 'Y' : 'N';  // Return whether any match was found
}

// Search by Salary with condition (0: ==, 1: >=)
char searchBySalary(Struct_Employee_Info employees[], int count, int salary, int condition) {
    int found = 0;
    for (int i = 0; i < count; i++) {
        if ((condition == 0 && employees[i].salary == salary) ||  // == condition
            (condition == 1 && employees[i].salary >= salary)) {  // >= condition
            printf("Name: %s %s\tZip Code: %s\tDepartment: %s\tSalary: %d\n",
                   employees[i].firstName, employees[i].lastName, employees[i].zipCode, employees[i].department, employees[i].salary);
            found = 1;
        }
    }
    return found ? 'Y' : 'N';  // Return whether any match was found
}

// Trim leading/trailing whitespace
void trimWhitespace(char *str) {
    char *end;
    
    // Trim leading space
    while(isspace((unsigned char)*str)) str++;

    // Trim trailing space
    if(*str == 0) return;

    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator
    *(end+1) = 0;
}

int main() {
    FILE *file;
    Struct_Employee_Info employees[MAX_SIZE];
    char line[MAX_SIZE];
    int count = 0;

    // Open the file
    file = fopen("records.csv", "r");
    if (file == NULL) {
        printf("Unable to open the file.\n");
        return 1;
    }

    // Read data from file into employees array
    while (fgets(line, sizeof(line), file)) {
        // Parse line using strtok (for comma-separated values)
        char *firstName = strtok(line, "\t");
        char *lastName = strtok(NULL, "\t");
        char *zipCode = strtok(NULL, "\t");
        char *department = strtok(NULL, "\t");
        char *salaryStr = strtok(NULL, "\t");

        // Ensure data is correctly read and remove extra spaces
        if (firstName && lastName && zipCode && department && salaryStr) {
            trimWhitespace(firstName);
            trimWhitespace(lastName);
            trimWhitespace(zipCode);
            trimWhitespace(department);
            strcpy(employees[count].firstName, firstName);
            strcpy(employees[count].lastName, lastName);
            strcpy(employees[count].zipCode, zipCode);
            strcpy(employees[count].department, department);
            employees[count].salary = atoi(salaryStr);  // Convert salary to integer
            count++;
        }
    }
    fclose(file);

    // Search by Name
    printf("Search Results by Name: Jack Sparrow\n");
    if (searchByName(employees, count, "Jack", "Sparrow") == 'N') {
        printf("No matching records found.\n");
    }

    // Search by Zip Code
    printf("\nSearch Results by Zip Code: 37128\n");
    if (searchByZipCode(employees, count, "37128") == 'N') {
        printf("No matching records found.\n");
    }

    // Search by Salary (>= 45000)
    printf("\nSearch Results by Salary: >= 45000\n");
    if (searchBySalary(employees, count, 45000, 1) == 'N') {
        printf("No matching records found.\n");
    }

    // Search by Salary (== 500000)
    printf("\nSearch Results by Salary: == 500000\n");
    if (searchBySalary(employees, count, 500000, 0) == 'N') {
        printf("No matching records found.\n");
    }

    return 0;
}
