
#include <immintrin.h>

void function(int n, int M, double *A, double *B)
{
  int vec_size = 4; // vec_size = 4 since AVX can process 4 double values
  __m256d vec1, vec2, factor; 
  for (int i = 0; i < n; i += M)  
  {
    for (int j = 0; j < n; j += M) 
    {
      for (int ii = i; ii < i + M; ii++)
      {
        factor = _mm256_set1_pd(A[ii * n + j]);
        int jj = j;
        for (; jj <= j + M - vec_size; jj += vec_size) // Vectorized loop
        {
          vec1 = _mm256_loadu_pd(&A[jj * n + ii]);
          vec2 = _mm256_loadu_pd(&B[ii * n + jj]);
          vec2 = _mm256_fnmadd_pd(factor, vec1, vec2);
          _mm256_storeu_pd(&B[ii * n + jj], vec2);
        }
        for (; jj < j + M; jj++) // Handle remaining elements
        {
          B[ii * n + jj] -= A[ii * n + j] * A[j * n + jj];
        }
      }
    }
  }
}
