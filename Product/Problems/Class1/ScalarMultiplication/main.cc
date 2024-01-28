#include <immintrin.h>
#include <random>
#include <iostream>

const int n = 48;         // size of matrix
double a;                 // scalar
double B[n];              // vector
double C[n];              // resulting vector

void function(int n, double a, double *b, double *c);


void initialize() // set vector and scalar
{
    srand(0);
    a = (double)rand() / (double)rand();
    for (int i = 0; i < n; i++)
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
    function(n,a,B,C);
    display();
    return 0;
}