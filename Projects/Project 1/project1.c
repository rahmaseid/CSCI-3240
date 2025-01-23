//Rahma Seid
//CSCI 3240
//Task: Write a C program that takes a single command-line argument and produces the same output as the xxd program with the same argument.
//Texts files used: test.txt

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char *argv[]){
    if (argc != 2){
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    // Open file
    FILE *file = fopen(argv[1], "rb"); // 'rb' opens in binary mode
    if (file == NULL){
        printf("Error: Could not open file %s\n", argv[1]);
        return 1;
    }

    unsigned char buffer[16]; // Array to hold 16 characters (16 bytes per line)
    int index;
    int bytes_read;
    int offset = 0; // Tracks the position in the file

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        // Print the offset (8 digits, hexadecimal, followed by a colon)
        printf("%08x: ", offset);

        // Print hex values in two columns of 8 bytes
        for (index = 0; index < 16; index++) {
            if (index < bytes_read) {
                // Print each byte in hex format
                printf("%02x", buffer[index]);
            } else {
                // Print spaces for padding when fewer than 16 bytes
                printf("  ");
            }


            if (index % 2 == 1) {
                printf(" "); // Add a space after every two bytes
            }
        }

        // Add space between hex dump and printable characters
        printf(" ");

        // Print printable characters or '.' for non-printables
        for (index = 0; index < bytes_read; index++) {
            if (isprint(buffer[index])) {
                printf("%c", buffer[index]); // Printable character
            } else {
                printf("."); // Non-printable character
            }
        }

        // New line for the next line of output
        printf("\n");
        // Increase offset by 16 for the next line of bytes
        offset += 16;
    }

    // Close the file
    fclose(file);
    return 0;
}
