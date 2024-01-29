#include <immintrin.h>
#include <iostream>

const int n = 16;    // size of matrix, should be a multiple of M
const int M = 8;     // size of blocks
double A[n * n];     // source matrix
double B[n * n];     // destination matrix

void function(int n, int M, double *A, double *B);


void initialize()
{
    for (int i = 0; i < n * n; i++)
        A[i] = B[i] = i;
}

void display()
{
    for (int i = 0; i < n * n; i++)
        std::cout << B[i] << " ";
}


int main()
{
    initialize();
    function(n,M,A,B);
    display();
    return 0;
}