//Rahma Seid
//Lab 5: Your task is to get the student data from the file, using structures
//Files needed: students.csv

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 11111
#define MAX_NAME_LENGTH 51
#define MAX_ID_LENGTH 7


// Structure to represent student info
typedef struct{
    char StudentName[MAX_NAME_LENGTH];
    char ID[MAX_ID_LENGTH];
    float Score;
} Struct_Student_Info;

//Function Prototypes
int CompareStudents(const void* a, const void* b); // Comparison function for qsort
void Display(Struct_Student_Info studentsArray[], int n); //Function to displat sorted students

int main(){
    FILE *file = fopen("students.csv", "r");
    if (file == NULL){
        printf("Error opening file");
        return 1;
    }

    Struct_Student_Info studentsArray[MAX_STUDENTS];
    int n = 0;
    char buffer[100]; //Buffer to hold each line in the file

    //Read student data from csv file
    fgets(buffer, sizeof(buffer), file);
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        sscanf(buffer, "%[^\t]\t%[^\t]\t%f", studentsArray[n].StudentName, studentsArray[n].ID, &studentsArray[n].Score);
        n++;
        if (n >= MAX_STUDENTS) break; // Prevent exceeding array bounds
    }
    fclose(file);

    //Sort students by score & name using qsort
    qsort(studentsArray, n, sizeof(Struct_Student_Info), CompareStudents);

    //Display sorted students
    Display(studentsArray, n);

    return 0;
}

int CompareStudents(const void* a, const void* b){ // Comparison function for qsort
    Struct_Student_Info *studentA = (Struct_Student_Info *)a;
    Struct_Student_Info *studentB = (Struct_Student_Info *)b;

    // First, compare by score
    if (studentA->Score < studentB->Score) {
        return -1;
    } else if (studentA->Score > studentB->Score) {
        return 1;
    }

    //If scores are equal, compare alphabetically by name
    return strcmp(studentA->StudentName, studentB->StudentName);
}

void Display(Struct_Student_Info studentsArray[], int n){
    for (int i = 0; i < n; i++){
        printf("%s %s %.2f\n", studentsArray[i].StudentName, studentsArray[i].ID, studentsArray[i].Score);
    }
}