#include <immintrin.h>
#include <random>
#include <iostream>

const int n = 30;              // size of arrays
int A[n], B[n], C[n], D[n];    // input arrays
int E[n];                      // destination array


void function(int n, int *A, int *B, int *C, int* D, int* E);


void initialize() // set vectors and scalars
{
    srand(0);
    for (int i = 0; i < n; i++) {
        A[i] =  rand();
        B[i] =  rand();
        C[i] =  rand();
        D[i] =  rand();
    }
}

void display()
{
    for (int i = 0; i < n ; i++)
    {
        std::cout << C[i] << " ";
    }
    std::cout << std::endl;
}


int main()
{
    initialize();
    function(n,A,B,C,D,E);
    display();
    return 0;
}