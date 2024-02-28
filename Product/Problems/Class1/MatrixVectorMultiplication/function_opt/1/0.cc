
#include <immintrin.h>
void function(int row, int col, double *a, double *b, double *c)
{
    for (int k = 0; k < row; k++)
    {
        c[k] = 0;
        int i = 0;
        __m256d sum = _mm256_setzero_pd(); // Initialize the sum to zero using AVX
        for (; i <= col - 4; i += 4)
        {
            __m256d vec_a = _mm256_loadu_pd(&a[k * col + i]);
            __m256d vec_b = _mm256_loadu_pd(&b[i]);
            sum = _mm256_fmadd_pd(vec_a, vec_b, sum); // Multiply-add operation using AVX
        }

        // Horizontal sum within AVX register
        __m256d sum_add = _mm256_hadd_pd(sum, sum);
        __m128d lo = _mm256_castpd256_pd128(sum_add);
        __m128d hi = _mm256_extractf128_pd(sum_add, 1);
        __m128d sum_lo_hi = _mm_add_pd(lo, hi);

        // Horizontal sum of 128-bit halves
        double res[4];
        _mm_storeu_pd(res, sum_lo_hi);

        // Update total sum in c[k]
        for (int j = 0; j < 4; j++)
        {
            c[k] += res[j];
        }

        // Handle the remaining elements
        for (; i < col; i++)
        {
            c[k] += a[k * col + i] * b[i];
        }
    }
}
