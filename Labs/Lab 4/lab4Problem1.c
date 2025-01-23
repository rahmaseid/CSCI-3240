//Rahma Seid
//Course: CSCI 3240
//Lab 4: Read a file character by character and store it in an array of characters of size 11 (the last index should contain “\0”)

#include <stdio.h>
#include <stdlib.h> // For exit function
#include <ctype.h>  // For isprint function

int main(int argc, char *argv[]){
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    
    //Open the file
    FILE *file = fopen(argv[1], "r");
    if (file == NULL){
        printf("Error: Could not open file %s\n", argv[1]);
        return 1;
    }

    char buffer[11]; //Array to hold 10 characters, plus null terminator
    int index = 0;
    int ch;

    //Read the character character by character
    while ((ch = fgetc(file)) != EOF){
        if(isprint(ch)){  //check if character is printable
            buffer[index] = ch; //Store the character
        } else{
            buffer[index] = '.'; //Replace none printable character with .
        }
        index++;

        //If there are more than 10 characters, print & reset the index
        if (index == 10){
            buffer[index] = '\0'; //Add null terminator to make it a string
            printf("%s\n", buffer); //Print string
            index = 0; //Reset index for next line
        }
    }

    //If there are remaining chracters in the buffer, print them
    if(index > 0){
        buffer[index] = '\0'; //Add null terminator to make it string
        printf("%s\n", buffer);
    }

    //Close the file
    fclose(file);
    return 0;
}