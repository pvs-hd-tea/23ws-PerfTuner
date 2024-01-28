#include <immintrin.h>
#include <random>
#include <iostream>

const int n = 48;         // size of matrix
double A[n],B[n];         // vectors 1 and 2
double C[n];              // resulting vector

void function(int n, double *a, double *b, double *c);


void initialize() // set vectors
{
    srand(0);
    for (int i = 0; i < n; i++)
    {
        A[i] = (double)rand() / (double)rand();
        B[i] = (double)rand() / (double)rand();
    }
}

void display()
{
    for (int i = 0; i < n; i++)
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