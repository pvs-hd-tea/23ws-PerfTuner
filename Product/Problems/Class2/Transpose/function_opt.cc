
#include <immintrin.h>

void function(int n, int M, double* A, double* B)
{
    for (int i = 0; i < n; i += M)   // split i loop in blocks of size M
    {
        for (int j = 0; j < n; j += M) // split j loop in blocks of size M
        {
            for (int ii = 0; ii < M; ii += 4) // process 4 elements at a time using AVX
            {
                for (int jj = 0; jj < M; jj += 4) // process 4 elements at a time using AVX
                {
                    int b_start = (i + ii) + n * (j + jj);
                    int a_start = (j + jj) + n * (i + ii);

                    __m256d a0 = _mm256_loadu_pd(&A[a_start]);
                    __m256d a1 = _mm256_loadu_pd(&A[a_start + n]);
                    __m256d a2 = _mm256_loadu_pd(&A[a_start + 2 * n]);
                    __m256d a3 = _mm256_loadu_pd(&A[a_start + 3 * n]);

                    __m256d t0 = _mm256_unpacklo_pd(a0, a1);
                    __m256d t1 = _mm256_unpacklo_pd(a2, a3);
                    __m256d t2 = _mm256_unpackhi_pd(a0, a1);
                    __m256d t3 = _mm256_unpackhi_pd(a2, a3);

                    __m256d b0 = _mm256_permute2f128_pd(t0, t1, 0x20);
                    __m256d b1 = _mm256_permute2f128_pd(t2, t3, 0x20);
                    __m256d b2 = _mm256_permute2f128_pd(t0, t1, 0x31);
                    __m256d b3 = _mm256_permute2f128_pd(t2, t3, 0x31);

                    // Use non-temporal stores
                    _mm256_stream_pd(&B[b_start], b0);
                    _mm256_stream_pd(&B[b_start + n], b1);
                    _mm256_stream_pd(&B[b_start + 2 * n], b2);
                    _mm256_stream_pd(&B[b_start + 3 * n], b3);
                }
            }
        }
    }
}
