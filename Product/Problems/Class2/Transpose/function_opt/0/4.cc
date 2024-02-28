
#include <immintrin.h>

void function(int n, int M, double *A, double *B)
{
  int vec_size = 4;
  __m256d factor;
  for (int i = 0; i < n; i += M)   // split i loop in blocks of size M
  {
    for (int j = 0; j < n; j += M) // split j loop in blocks of size M
    {
      factor = _mm256_set1_pd(A[j * n + i]); // calculate factor outside the loop
      
      for (int ii = i; ii < i + M; ii++)
      {
        int m = ii * n;
        int nIdx = j * n;
        __m256d vec1, vec2;
        
        for (int jj = j; jj < j + M; jj += vec_size) // head loop (optimized using AVX)
        {
          vec1 = _mm256_loadu_pd(&A[nIdx+ii]);
          vec2 = _mm256_loadu_pd(&B[jj * n + ii]);
          vec2 = _mm256_fnmadd_pd(factor, vec1, vec2);
          _mm256_storeu_pd(&B[jj * n + ii], vec2);
        }

        for (; jj < j + M; jj++) // handle the remaining elements
          B[jj * n + ii] -= A[j * n + i] * A[nIdx + ii];
      }
    }
  }
}
