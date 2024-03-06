#include <immintrin.h>
void function(int row, int col, double *a, double *b, double *c)
{
    for (int k = 0; k < row; k++)
    {
        c[k] = 0;
        for (int i = 0; i < col; i++)
        {
            c[k] += a[k * col + i] * b[i];
        }
    }
}