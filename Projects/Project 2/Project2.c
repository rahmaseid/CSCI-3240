/* 
 * CSCI3240: Project 2
 * 
 * Name: Rahma Seid
 * Project2.c - Source file with your solutions to the Lab.
 *          This is the file you will submit in the D2L -> Project2 dropbox.
 *
 *   Please make sure you don't include the main function in this file. Graders will use a separate `.c` file containing the `main` function to test your Mystery functions.  
 *	 If you'd like to test your code, you can create a separate `main.c` file that includes the `main` function, which calls the Mystery functions defined in this `Project2.c` file.
 *   You don't need to submit the 'main.c' file.
 *	 Make sure you have insightful comments in your code. 
 *   
 * Files used: Project2.s
 */


long MysteryFunction1(long a, int b){
    long result = 1; // Initialize result to 1, as any number to the power of 0 is 1
    while (b > 0){
        result *= a; // Multiply the result by a
        b--; //Decrease b by 1
    }
    return result;
}

// Reverses the bits of an unsigned integer
unsigned int MysteryFunction2(unsigned int num){
    unsigned int result = 0;
    for (int i = 0; i < 32; i++){
        if (num &(1 << i)){
            result |= (1 << (31 - i)); // Set the corresponding reversed bit
        }
    }
    return result; // Returns the number with its bits reversed
}

// Finds the maximum value in an array of long integers
long MysteryFunction3(long *a, int n){
    long max = a[0];  // Initialize max to the first element of the array
    for (int i = 1; i < n; i++){
        if (a[i] > max){
            max = a[i];
        }
    }
    return max; //Return maximum value
}

// Counts the number of set bits (1s) in an unsigned long integer
int MysteryFunction4(unsigned long n){
    int count = 0; // Initialize count to 0
    while (n > 0){
        count += n&1;
        n>>=1; // Right shift n to process the next bit
    }
    return count;
}


// Counts the number of differing bits between two unsigned integers
unsigned int MysteryFunction5(unsigned int A, unsigned int B){
    unsigned int count = 0;
    unsigned int result = A ^ B;
    while(result > 0){
        count += result&1;
        result >>= 1;
    }
    return count;
}

// Do not add a main function here.

