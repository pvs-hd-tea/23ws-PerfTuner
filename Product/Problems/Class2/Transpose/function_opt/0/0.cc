
#include <immintrin.h>

void function(int n, int M, double *A, double *B)
{
  int vec_size = 4; // vec_size = 4 since the AVX can hold 4 double values
  __m256d vec1, vec2, factor; // define variables only once outside the loop

  for (int i = 0; i < n; i += M)
  {
    for (int j = 0; j < n; j += M)
    {
      for (int ii = i; ii < i + M; ii++)
      {
        for (int jj = j; jj < j + M; jj += vec_size) // vectorized j loop
        {
          factor = _mm256_set1_pd(A[ii * n + jj + vec_size - 1]); // load constant factor once for the entire block
          
          int idx = jj * n + ii; // calculate the starting index for vectorized operations
          for (int k = 0; k < vec_size; k++) // vectorize the computation
          {
            vec1 = _mm256_loadu_pd(&A[(ii * n) + jj + k]); // load 4 elements of A
            vec2 = _mm256_loadu_pd(&B[idx + k * n]);       // load 4 elements from B
            vec2 = _mm256_fnmadd_pd(factor, vec1, vec2);    // perform vectorized operation
            _mm256_storeu_pd(&B[idx + k * n], vec2);        // store the result back to B
          }
        }
      }
    }
  }
}
