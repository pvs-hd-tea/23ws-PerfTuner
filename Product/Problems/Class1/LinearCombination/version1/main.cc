#include <immintrin.h>
#include <random>
#include <iostream>

const int n = 50;         // size of matrix
double a,c;               // scalars 1 and 2
double B[n],D[n];         // vectors 1 and 2
double E[n];              // resulting vector

void function(int n, double a, double *b, double c, double *d, double *e);


void initialize() // set vectors and scalars
{
    srand(0);
    a = (double)rand() / (double)rand();
    c = (double)rand() / (double)rand();
    for (int i = 0; i < n; i++)
    {
        B[i] = (double)rand() / (double)rand();
        D[i] = (double)rand() / (double)rand();
    }
}

void display()
{
    for (int i = 0; i < n; i++)
    {
        std::cout << E[i] << " ";
    }
    std::cout << std::endl;
}


int main()
{
    initialize();
    function(n,a,B,c,D,E);
    display();
    return 0;
}