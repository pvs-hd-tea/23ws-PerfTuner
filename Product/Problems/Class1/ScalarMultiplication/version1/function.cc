#include <immintrin.h>
void function(int n, double a, double *b, double *c)
{
    for (int i = 0; i < n; i++)
    {
        c[i] = a * b[i];
    }
}