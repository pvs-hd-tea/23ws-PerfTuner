To optimize the function using AVX intrinsics, we can leverage the SIMD capabilities of AVX to perform vectorized operations on multiple elements at once.

To do this, we can modify the inner loop of both snippet1 and snippet2 to process multiple elements at once using AVX intrinsics.

Here is the optimized function:

```cpp
#include <immintrin.h>

void lu_3_opt()
{
  const int vec_size = 4; // vec_size = 4 since the AVX can hold 4 double values
  const int n_mod_b = n - (n % B);
  
  for (int K = 0; K < n_mod_b; K += B) // K-blocks
  {
    // upper left block
    for (int k = K; k < K + B; k += 1)       // k index within block
    {
      for (int i = k + 1; i < K + B; i += 1) // head loop for i
      {
        A[i][k] /= A[k][k];
        
        __m256d factor = _mm256_set1_pd(A[i][k]);
        
        int j = k + 1;
        for (; j <= K + B - vec_size; j += vec_size) // head loop for j (vectorized)
        {
          __m256d vec1 = _mm256_loadu_pd(&A[k][j]);
          __m256d vec2 = _mm256_loadu_pd(&A[i][j]);
          vec2 = _mm256_fnmadd_pd(factor, vec1, vec2);
          _mm256_storeu_pd(&A[i][j], vec2);
        }
        
        for (; j < K + B; j += 1) // tail loop for j
        {
          A[i][j] -= A[i][k] * A[k][j];
        }
      }
    }

    // U blocks in first block row
    for (int J = K + B; J < n_mod_b; J += B) // block loop in j-direction
    {
      for (int k = K; k < K + B; k += 1) // k index within block
      {
        for (int i = k + 1; i < K + B; i += 1) // head loop for i
        {
          for (int j = J; j < J + B; j += 1) // j-loop within block
          {
            A[i][j] -= A[i][k] * A[k][j]; // consecutive access
          }
        }
      }
    }

    // L blocks in first block column
    for (int I = K + B; I < n_mod_b; I += B) // block loop in i-direction
    {
      for (int k = K; k < K + B; k += 1) // k index within block
      {
        for (int i = I; i < I + B; i += 1) // i-loop within block
        {
          A[i][k] /= A[k][k];
          
          __m256d factor = _mm256_set1_pd(A[i][k]);
          
          int j = k + 1;
          for (; j <= K + B - vec_size; j += vec_size) // head loop for j (vectorized)
          {
            __m256d vec1 = _mm256_loadu_pd(&A[k][j]);
            __m256d vec2 = _mm256_loadu_pd(&A[i][j]);
            vec2 = _mm256_fnmadd_pd(factor, vec1, vec2);
            _mm256_storeu_pd(&A[i][j], vec2);
          }
          
          for (; j < K + B; j += 1) // tail loop for j
          {
            A[i][j] -= A[i][k] * A[k][j];
          }
        }
      }
    }
    
    // the rest matrix; here is the bulk of the work
    for (int I = K + B; I < n_mod_b; I += B) // block loop in i-direction
    {
      for (int J = K + B; J < n_mod_b; J += B) // block loop in j-direction
      {
        for (int i = I; i < I + B; i += 1) // i-loop within block
        {
          int j = J;
          for (; j <= J + B - vec_size; j += vec_size) // head loop for j (vectorized)
          {
            for (int k = K; k < K + B; k += 1) // k index within block
            {
              __m256d vec1 = _mm256_loadu_pd(&A[i][k]);
              __m256d vec2 = _mm256_loadu_pd(&A[k][j]);
              __m256d prod = _mm256_mul_pd(vec1, vec2);
              vec2 = _mm256_sub_pd(_mm256_loadu_pd(&A[i][j]), prod);
              _mm256_storeu_pd(&A[i][j], vec2);
            }
          }
          
          for (; j < J + B; j += 1) // tail loop for j
          {
            for (int k = K; k < K + B; k += 1) // k index within block
            {
              A[i][j] -= A[i][k] * A[k][j];
            }
          }
        }
      }
    }
  }
}
```

In this optimized function, we have replaced the innermost loops in snippet1 and snippet2 with AVX intrinsics to perform vectorized operations on multiple elements at once. This allows for better utilization of the CPU's SIMD capabilities and can lead to significant performance improvements.