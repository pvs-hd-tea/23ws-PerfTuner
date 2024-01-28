#include <immintrin.h>
void function(int n, double *a, double *b, double &c)
{
    c = 0;
    for (int i = 0; i < n; i++)
    {
        c += a[i] * b[i];
    }
}