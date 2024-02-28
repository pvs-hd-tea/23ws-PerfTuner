
#include <immintrin.h>
void function(int row, int col, double *a, double *b, double *c)
{
    for (int k = 0; k < row; k+=4)
    {
        c[k] = 0;
        for (int i = 0; i < col; i+=4)
        {
            int c_start = k;
            int a_start = k * col + i;
            int b_start = i;

            __m256d c0 = _mm256_loadu_pd(&c[c_start]);
            __m256d c1 = _mm256_loadu_pd(&c[c_start + 4]);
            __m256d c2 = _mm256_loadu_pd(&c[c_start + 8]);
            __m256d c3 = _mm256_loadu_pd(&c[c_start + 12]);

            for (int j = 0; j < 4; j++)
            {
                __m256d a_val = _mm256_loadu_pd(&a[a_start + j]);
                __m256d b_val = _mm256_set1_pd(b[b_start + j]);

                c0 = _mm256_fmadd_pd(a_val, b_val, c0);
            }

            _mm256_stream_pd(&c[c_start], c0);
            _mm256_stream_pd(&c[c_start + 4], c1);
            _mm256_stream_pd(&c[c_start + 8], c2);
            _mm256_stream_pd(&c[c_start + 12], c3);
        }
    }
}
