
#include <immintrin.h>
void function(int n, int M, double *A, double *B)
{
  int vec_size = 4; 
  __m256d vec1, vec2, factor;
  for (int i = 0; i < n; i += M)
  {
    for (int j = 0; j < n; j += M)
    {
      factor = _mm256_set1_pd(A[i * n + j]);
      int ii = i;
      for (; ii < i + M; ii += vec_size) 
      {
        int jj = j;
        for (; jj < j + M; jj += vec_size) 
        {
          vec1 = _mm256_loadu_pd(&A[jj * n + ii]);
          vec2 = _mm256_loadu_pd(&B[ii * n + jj]);
          vec2 = _mm256_fnmadd_pd(factor, vec1, vec2);
          _mm256_storeu_pd(&B[ii * n + jj], vec2);
        }
        for (; jj < j + M; jj += 1)
        {
          B[ii * n + jj] -= A[i * n + j] * A[jj * n + ii];
        }
      }
      
    }
  }
}
