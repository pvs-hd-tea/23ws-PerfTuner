
#include <immintrin.h>
void function(int row, int col, double *a, double *b, double *c)
{
    int vec_size = 4; // AVX vector size for 4 double values
    __m256d vecA, vecB, vecC, factor;

    for (int k = 0; k < row; k++)
    {
        c[k] = 0;
        for (int i = 0; i < col; i += vec_size)
        {
            vecA = _mm256_loadu_pd(&a[k * col + i]);
            vecB = _mm256_loadu_pd(&b[i]);
            factor = _mm256_set1_pd(vecB[0]); // Broadcast first element of vecB
            vecC = _mm256_loadu_pd(&c[k]);
            vecC = _mm256_fmadd_pd(vecA, factor, vecC); // Multiply-Add operation with FMADD
            _mm256_storeu_pd(&c[k], vecC);
        }

        // Handle remaining elements
        for (int i = col - (col % vec_size); i < col; i++)
        {
            c[k] += a[k * col + i] * b[i];
        }
    }
}
