
#include <immintrin.h>
void function(int row, int col, double *a, double *b, double *c)
{
    for (int k = 0; k < row; k++)
    {
        c[k] = 0;
        for (int i = 0; i < col; i+=4)
        {
            __m256d sum = _mm256_setzero_pd();
            for (int j = 0; j < 4; j++)
            {
                __m256d vec_a = _mm256_loadu_pd(&a[k * col + i + j]);
                __m256d vec_b = _mm256_broadcast_sd(&b[i + j]);
                sum = _mm256_fmadd_pd(vec_a, vec_b, sum);
            }
            double temp[4];
            _mm256_storeu_pd(temp, sum);
            c[k] += temp[0] + temp[1] + temp[2] + temp[3];
        }
    }
}
