//Rahma Seid
//Course: CSCI 3240
//Lab 3: Read the numbers in an array and reverse the order of the elements

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n, *arr, i;
    //getting the size of array
    scanf("%d", &n);
    arr = (int*) malloc(n * sizeof(int));

    //getting the element of the array
    for(i = 0; i < n; i++) {
        scanf("%d", arr + i);
    }

    //TODO: Write the logic to reverse the array "arr" here  
    for (i = 0; i < n / 2; i++) {
        int temp = *(arr + i);                  // Store the current element
        *(arr + i) = *(arr + (n - 1 - i));      // Swap the elements
        *(arr + (n - 1 - i)) = temp;            // Complete the swap
    }

    
    //END TODO
      
    // Printing the resulting reversed array
    for(i = 0; i < n; i++)
        printf("%d ", *(arr + i));
    printf("\n");
    return 0;
}