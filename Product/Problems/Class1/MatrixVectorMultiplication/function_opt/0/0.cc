
#include <immintrin.h>
void function(int row, int col, double *a, double *b, double *c)
{
    for (int k = 0; k < row; k+=4)
    {
        __m256d sum = _mm256_setzero_pd();
        for (int i = 0; i < col; i+=4)
        {
            int row_offset = k * col + i;
            __m256d vec_a = _mm256_loadu_pd(&a[row_offset]);

            int j;
            for(j=0; j<4; ++j) {
                __m256d vec_b = _mm256_broadcast_sd(&b[j]);
                sum = _mm256_fmadd_pd(vec_a, vec_b, sum);
                vec_b = _mm256_broadcast_sd(&b[j+1]);
                sum = _mm256_fmadd_pd(vec_a, vec_b, sum);
                vec_b = _mm256_broadcast_sd(&b[j+2]);
                sum = _mm256_fmadd_pd(vec_a, vec_b, sum);
                vec_b = _mm256_broadcast_sd(&b[j+3]);
                sum = _mm256_fmadd_pd(vec_a, vec_b, sum);
            }
        }
        _mm256_store_pd(&c[k], sum);
    }
}
