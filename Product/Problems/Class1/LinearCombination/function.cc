#include <immintrin.h>
void function(int n, double a, double *b, double c, double *d, double *e)
{
    for (int i = 0; i < n; i++)
    {
        e[i] = a * b[i] + c * d[i];
    }
}