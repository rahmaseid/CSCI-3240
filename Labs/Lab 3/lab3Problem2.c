//Rahma Seid
//Course: CSCI 3240
//Lab 3: Read a line of text from the user and count the number of words. For each word, if the word starts with a vowel, capitalize the word and display it in ascending order.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 50

//Function prototypes
int compare(const void *a, const void *b);
void capitalize_vowels(char *word);

int main(){
    char input[1024];
    int count = 0;
    char *words[MAX_WORDS]; //array to hold pointers to words
    
    printf("Enter a line of text: ");
    fgets(input, sizeof(input), stdin);

    //
    char *token = strtok(input, " \t\n");
    while (token != NULL && count < MAX_WORDS){
        words[count] = malloc(strlen(token) + 1);
        strcpy(words[count], token);
        token = strtok(NULL, " \t\n");

        count++;
    }

    //Sort words in ascending order
    qsort(words, count, sizeof(char *), compare);
    
    printf("Words starting with vowels: \n");
    for (int i =0; i < count; i++){
        if (strchr("aeiouAEIOU", (words[i][0]))) {   // Check if the first letter is uppercase
            capitalize_vowels(words[i]); // Capitalize if it starts with a vowel
            printf("%s\n", words[i]);
        }
        free(words[i]);  // Free allocated memory for each word
    }
}

//compare funnction for qsort
int compare(const void *a, const void *b){
    const char *str1 = *(const char **)a;
    const char *str2 = *(const char **)b;
    return strcmp(str1, str2);
}

//Function to capitalize all letters that start with a vowel
void capitalize_vowels(char *word){
    for (int i=0; word[i]; i++){
        word[i] = toupper(word[i]);
    }
}

