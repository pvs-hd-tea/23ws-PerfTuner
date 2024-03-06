#include <immintrin.h>
#include <random>
#include <iostream>

const int n = 30;    // size of arrays
int A[n], B[n];      // input arrays
int C[n];            // destination array


void function(int n, int *A, int *B, int *C);


void initialize() // set vectors and scalars
{
    srand(0);
    for (int i = 0; i < n; i++) {
        A[i] =  rand();
        B[i] =  rand();
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
    function(n,A,B,C);
    display();
    return 0;
}