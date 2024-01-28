#include <immintrin.h>
#include <random>
#include <iostream>

const int n = 24; // number of rows of matrix A
const int m = 8;  // number of columns of matrix A and number of rows of vector B
double A[n * m];  // matrix A
double B[m];      // vector B
double C[n];      // resulting vector

void function(int row, int col, double *a, double *b, double *c);

void initialize() // set matrix and vector
{
    srand(0);
    for (int i = 0; i < n * m; i++)
    {
        A[i] = (double)rand() / (double)rand();
    }
    for (int i = 0; i < m; i++)
    {
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
    function(n, m, A, B, C);
    display();
    return 0;
}
