
#include <immintrin.h>
void function(int n, double a, double *b, double *c)
{
    int i, j;
    for (i = 0; i < n; i += 4)
    {
        for (j = 0; j < n; j += 4)
        {
            int b_start = j + i * n;
            int a_start = i + j * n;

            __m256d a0 = _mm256_loadu_pd(&b[a_start]);
            __m256d a1 = _mm256_loadu_pd(&b[a_start + 1]);
            __m256d a2 = _mm256_loadu_pd(&b[a_start + 2]);
            __m256d a3 = _mm256_loadu_pd(&b[a_start + 3]);

            __m256d t0 = _mm256_unpacklo_pd(a0, a1);
            __m256d t1 = _mm256_unpacklo_pd(a2, a3);
            __m256d t2 = _mm256_unpackhi_pd(a0, a1);
            __m256d t3 = _mm256_unpackhi_pd(a2, a3);

            __m256d b0 = _mm256_permute2f128_pd(t0, t1, 0x20);
            __m256d b1 = _mm256_permute2f128_pd(t2, t3, 0x20);
            __m256d b2 = _mm256_permute2f128_pd(t0, t1, 0x31);
            __m256d b3 = _mm256_permute2f128_pd(t2, t3, 0x31);

            _mm256_stream_pd(&c[b_start], b0);
            _mm256_stream_pd(&c[b_start + n], b1);
            _mm256_stream_pd(&c[b_start + 2 * n], b2);
            _mm256_stream_pd(&c[b_start + 3 * n], b3);
        }
    }
}
