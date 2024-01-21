#include <immintrin.h>
#include <random>
#include <iostream>

const int n = 48;         // size of matrix
double A[n],B[n];         // vectors 1 and 2

void function(int n, double *a, double *b, double &c);


void initialize() // set matrix
{
    srand(0);
    for (int i = 0; i < n; i++)
    {
        A[i] = (double)rand() / (double)rand();
        B[i] = (double)rand() / (double)rand();
    }
}


int main()
{
    double c;
    initialize();
    function(n,A,B,c);
    std::cout << c;
    return 0;
}