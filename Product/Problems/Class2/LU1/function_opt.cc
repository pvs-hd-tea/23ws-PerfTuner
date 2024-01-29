
#include <immintrin.h>

void function(int n, double* A)
{
  const int M = 8; // Size of the submatrix to be copied

  // Divide the loop to take advantage of AVX SIMD operations
  for (int i = 0; i < n; i += M)
  {
    for (int j = 0; j < n; j += M)
    {
      // Use AVX intrinsics for optimized data movement
      for (int ii = 0; ii < M; ii += 4)
      {
        for (int jj = 0; jj < M; jj += 4)
        {
          int b_start = (j + jj) * n + (i + ii);
          int a_start = (i + ii) * n + (j + jj);

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

          // Use non-temporal stores for better memory performance
          _mm256_stream_pd(&A[b_start], b0);
          _mm256_stream_pd(&A[b_start + n], b1);
          _mm256_stream_pd(&A[b_start + 2 * n], b2);
          _mm256_stream_pd(&A[b_start + 3 * n], b3);
        }
      }
    }
  }
}
