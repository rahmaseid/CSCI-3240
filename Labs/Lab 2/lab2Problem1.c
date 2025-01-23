//Rahma Seid
//Course: CSCi 3240
//Lab 2: Print a pattern of numbers from 1 to n

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    if (argc != 2) {
        printf("Usage: %d <size>", *argv[0]);
        return 1;
    }
    int n = atoi(argv[1]);
    int size = 2*n-1;

    if (n < 1 || n > 9) {
        printf("Error: n must be between 1 and 9.\n");
        return 1;
    }

    for (int i=0; i < n; i++){ //Loops for rows in top half

        for (int j =0; j < size; j++){ //Loop for columns in top half
            int edge;

            //Account for area to the nearest edge in rows
            if (i < (size-i-1)){
                edge = i;
            } else{
                edge = (size-i-1);
            }

            //Account for distance to the nearest edge in columns
            if ( j < (size-j-1)){
                edge = edge < j ? edge : j;
            } else{
                edge = edge < (size - j - 1) ? edge : (size - j - 1);;
            }

            printf("%d ", n-edge);
        }
        printf("\n");
    }

    //Print bottom half of pattern
    for (int i = n - 2; i >= 0; i--) { // Loops for rows in the bottom half
        for (int j = 0; j < size; j++) { // Loop for columns in the bottom half
            int edge;
             
             //Account for distance to the nearest edge in rows
            if (i < (size-i-1)){
                edge = i;
            } else{
                edge = (size-i-1);
            }

            //Account for area to the nearest edge in columns
            if (j < (size-j-1)){
                edge = edge < j ? edge : j;;
            } else{
                edge = edge < (size - j - 1) ? edge : (size - j - 1);
            }
        
            printf("%d ", n-edge);
         }
        printf("\n");
        
    }
    
    return 0;
}



//for row in range (0, n):
        //for col in range (row, n-1)