#include <immintrin.h>
#include <random>
#include <iostream>

int const size = 14;   // size of array (size/2 complex numbers)
double A[size];        // array 1 with complex numbers
double B[size];        // array 2 with complex numbers
double C[size];        // destination array with complex numbers

void function(int n,double* a, double* b, double* c);

void initialize() // set arrays
{
    srand(0);
    for (int i = 0; i < size; i++)
    {
        A[i] = (double)rand() / (double)rand();
    }
    for (int i = 0; i < size; i++)
    {
        B[i] = (double)rand() / (double)rand();
    }
    for (int i = 0; i < size; i++)
    {
        C[i] = 0;
    }
}

void display()
{
    for (int i = 0; i < size; i+=2) {
        std::cout << C[i] << " + i * " << C[i+1] << std::endl;
    }
}

int main()
{
    initialize();
    function(size, A, B, C);
    display();
    return 0;
}
