#include <immintrin.h>
void function(int row1, int col1, int col2, double *a, double *b, double *c)
{
    for (int k = 0; k < row1; k++)
    {
        for (int j = 0; j < col2; j++)
        {
            c[k * row1 + j] = 0;
            for (int i = 0; i < col1; i++)
            {
                c[k * row1 + j] += a[k * col1 + i] * b[i * col2 + j];
            }
        }
    }
}