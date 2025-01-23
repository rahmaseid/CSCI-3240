//Rahma Seid
//Course: CSCI 3240
//Lab 4: Takes username and password from the user & compare username and password

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    char inputUsername[21]; //Array to hold input for username ( 20 characters &  '\0')
    char inputPassword[21]; //Array to hold input for password ( 20 characters &  '\0')

    //Get username & password from user
    printf("Enter username: ");
    scanf("%20s", inputUsername);

    printf("Enter password: ");
    scanf("%20s", inputPassword);

    FILE *file = fopen("userdata.txt", "r");
    if (file ==NULL){
        printf("Couldn't find file\n");
        return 1;
    }

    char fileUsername[21]; //To hold username from file
    char filePassword[21]; //To hold password from file
    double numData; //to hold numerical data from file

    int found = 0; //Flag to indicate if a match is found

    //Read each line from the file
    while(fscanf(file, "%20s %20s %lf", fileUsername, filePassword, &numData) != EOF){
        //compare input with file data
        if(strcmp(inputUsername, fileUsername) == 0 && strcmp(inputPassword, filePassword) == 0){
            printf("The amount is: %.2lf\n", numData); //Print numerical data
            found = 1; // Set the flag to signal a match
            break;
        }
    }

    // If no match was found, print the error message
    if (!found) {
        printf("username/password mismatched!!!\n");
    }

    //close file
    fclose(file);
    return 0;
        
}