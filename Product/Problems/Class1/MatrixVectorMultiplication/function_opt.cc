
#include <immintrin.h>

void function(int row, int col, double *a, double *b, double *c)
{
    const int M = 4; // Block size for optimization

    for (int i = 0; i < row; i += M)
    {
        for (int j = 0; j < col; j += M)
        {
            for (int ii = i; ii < i + M; ii += 4)
            {
                for (int jj = j; jj < j + M; jj += 4)
                {
                    int b_start = jj * col + ii;
                    int a_start = ii * col + jj;

                    __m256d a0 = _mm256_loadu_pd(&a[a_start]);
                    __m256d a1 = _mm256_loadu_pd(&a[a_start + col]);
                    __m256d a2 = _mm256_loadu_pd(&a[a_start + 2 * col]);
                    __m256d a3 = _mm256_loadu_pd(&a[a_start + 3 * col]);

                    __m256d t0 = _mm256_unpacklo_pd(a0, a1);
                    __m256d t1 = _mm256_unpacklo_pd(a2, a3);
                    __m256d t2 = _mm256_unpackhi_pd(a0, a1);
                    __m256d t3 = _mm256_unpackhi_pd(a2, a3);

                    __m256d b0 = _mm256_permute2f128_pd(t0, t1, 0x20);
                    __m256d b1 = _mm256_permute2f128_pd(t2, t3, 0x20);
                    __m256d b2 = _mm256_permute2f128_pd(t0, t1, 0x31);
                    __m256d b3 = _mm256_permute2f128_pd(t2, t3, 0x31);

                    // Use non-temporal stores
                    _mm256_stream_pd(&b[b_start], b0);
                    _mm256_stream_pd(&b[b_start + col], b1);
                    _mm256_stream_pd(&b[b_start + 2 * col], b2);
                    _mm256_stream_pd(&b[b_start + 3 * col], b3);
                }
            }
        }
    }
}
