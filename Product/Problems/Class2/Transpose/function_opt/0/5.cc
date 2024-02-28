
#include <immintrin.h>

void function(int n, int M, double *A, double *B)
{
  int vec_size = 4; // Vector size for AVX operations
  __m256d vec1, vec2, factor; // Define AVX variables
  int i, j, ii, jj;

  for (i = 0; i < n; i += M)   // split i loop in blocks of size M
  {
    for (j = 0; j < n; j += M) // split j loop in blocks of size M
    {
      factor = _mm256_set1_pd(A[n * i + s]); // Load A[s] into AVX register

      for (ii = i; ii < i + M; ii++)
      {
        for (jj = j; jj < j + M - vec_size; jj += vec_size) // Vectorized loop with AVX
        {
          vec1 = _mm256_loadu_pd(&A[n * ii + jj]);
          vec2 = _mm256_loadu_pd(&A[n * jj + ii]);
          vec2 = _mm256_fnmadd_pd(factor, vec1, vec2);
          _mm256_storeu_pd(&B[n * jj + ii], vec2);
        }

        for (; jj < j + M; jj++) // Handle remaining elements
        {
          B[n * jj + ii] = A[n * ii + jj];
        }
      }
    }
  }
}
