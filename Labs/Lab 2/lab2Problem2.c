//Rahma Seid
//Course: CSCi 3240
//Lab 2: Calculate the permutation and combination for given 𝒏 and 𝒓.

#include <stdio.h>
#include <stdlib.h>

//Calculate factorial
long long factorial (int num){
    if (num < 0){
        return 0;
    }
    long long fact = 1;
    for (int i = 1; i <=num; i++){
        fact *= i;
    }
    return fact;
}

//Calculate permutations (n,r)
long long permutation(int n, int r){
    if (n < r || n < 0 || r < 0){
        return 0;
    } else{
        return factorial(n)/factorial(n - r);
    }
}

//Calculate combinations (n,r)
long long combination(int n, int r){
    if (n < r || n < 0 || r < 0){
        return 0;
    } else{
        return permutation(n, r)/factorial(r);
    }
}

int main(int argc, char *argv[]){
    if (argc != 3){
        printf("Usage %s <n> <r> \n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int r = atoi(argv[2]);

    if (n < 0, r < 0){
        printf("Invalid input. Both inputs must be positive numbers");
        return 1;
    }

    long long perm = permutation(n,r);
    long long comb = combination(n,r);

    printf("P(%d,%d) = %lld \n", n, r, perm);
    printf("C(%d,%d) = %lld \n", n, r, comb);

    return 0;
    
}