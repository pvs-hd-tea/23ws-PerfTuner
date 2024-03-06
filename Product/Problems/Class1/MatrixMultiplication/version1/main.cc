#include <immintrin.h>
#include <random>
#include <iostream>

const int n = 26;          // number of rows of matrix A
const int m = 10;          // number of columns of matrix A and number of rows of matrix B
const int k = 10;          // number of columns of matrix B
double A[n * m], B[m * k]; // matrices A and B
double C[n * k];           // destination matrix and copy

void function(int row1, int col1, int col2, double *a, double *b, double *c);

void initialize() // set matrices
{
    srand(0);
    for (int i = 0; i < n * m; i++)
    {
        A[i] = (double)rand() / (double)rand();
    }
    for (int i = 0; i < m * k; i++)
    {
        B[i] = (double)rand() / (double)rand();
    }
}

void display()
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < k; j++)
        {
            std::cout << C[i * n + j] << " ";
        }
        std::cout << std::endl;
    }
}

int main()
{
    initialize();
    function(n, m, k, A, B, C);
    display();
    return 0;
}
