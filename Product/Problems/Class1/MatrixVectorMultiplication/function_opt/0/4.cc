
#include <immintrin.h>
void function(int row, int col, double *a, double *b, double *c)
{
    for (int k = 0; k < row; k++)
    {
        c[k] = 0;
        for (int i = 0; i < col; i+=4)
        {
            // Load 4 elements from array 'b'
            __m256d bv = _mm256_set1_pd(b[i]);

            // Accumulate in 64-bit chunks
            __m256d cv = _mm256_setzero_pd();
            for (int j = 0; j < 4; j++)
            {
                __m256d av = _mm256_loadu_pd(&a[k * col + i + j]);
                cv = _mm256_fmadd_pd(av, bv, cv);
            }

            // Accumulate the sum
            double sum[4];
            _mm256_storeu_pd(sum, cv);
            c[k] += sum[0] + sum[1] + sum[2] + sum[3];
        }
    }
}
