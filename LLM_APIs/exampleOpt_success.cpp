To optimize the given function using AVX intrinsics, we can use the provided snippet2 code as a reference. The key idea is to leverage AVX instructions to perform vectorized calculations on multiple double values at once, instead of processing them one by one in the nested loops.

Here is the optimized function:

```cpp
void lu_3_opt()
{
  int vec_size = 4; // AVX vector size

  for (int K = 0; K < n; K += B)
  {
    // upper left block
    for (int k = K; k < K + B; k += 1)
    {
      for (int i = k + 1; i < K + B; i += 1)
      {
        A[i][k] /= A[k][k];

        // Vectorized loop for j
        int j = k + 1;
        for (; j <= K + B - vec_size; j += vec_size)
        {
          __m256d factor = _mm256_set1_pd(A[i][k]);
          __m256d vec1 = _mm256_loadu_pd(&A[k][j]);
          __m256d vec2 = _mm256_loadu_pd(&A[i][j]);
          vec2 = _mm256_fnmadd_pd(factor, vec1, vec2);
          _mm256_storeu_pd(&A[i][j], vec2);
        }

        // Remainder loop for j
        for (; j < K + B; j += 1)
          A[i][j] -= A[i][k] * A[k][j];
      }
    }

    // U blocks in first block row
    for (int J = K + B; J < n; J += B)
    {
      for (int k = K; k < K + B; k += 1)
      {
        for (int i = k + 1; i < K + B; i += 1)
        {
          // Vectorized loop for j
          int j = J;
          for (; j <= J + B - vec_size; j += vec_size)
          {
            __m256d factor = _mm256_set1_pd(A[i][k]);
            __m256d vec1 = _mm256_loadu_pd(&A[k][j]);
            __m256d vec2 = _mm256_loadu_pd(&A[i][j]);
            vec2 = _mm256_fnmadd_pd(factor, vec1, vec2);
            _mm256_storeu_pd(&A[i][j], vec2);
          }

          // Remainder loop for j
          for (; j < J + B; j += 1)
            A[i][j] -= A[i][k] * A[k][j];
        }
      }
    }

    // L blocks in first block column
    for (int I = K + B; I < n; I += B)
    {
      for (int k = K; k < K + B; k += 1)
      {
        for (int i = I; i < I + B; i += 1)
        {
          A[i][k] /= A[k][k];

          // Vectorized loop for j
          int j = k + 1;
          for (; j <= K + B - vec_size; j += vec_size)
          {
            __m256d factor = _mm256_set1_pd(A[i][k]);
            __m256d vec1 = _mm256_loadu_pd(&A[k][j]);
            __m256d vec2 = _mm256_loadu_pd(&A[i][j]);
            vec2 = _mm256_fnmadd_pd(factor, vec1, vec2);
            _mm256_storeu_pd(&A[i][j], vec2);
          }

          // Remainder loop for j
          for (; j < K + B; j += 1)
            A[i][j] -= A[i][k] * A[k][j];
        }
      }
    }

    // the rest matrix
    for (int I = K + B; I < n; I += B)
    {
      for (int J = K + B; J < n; J += B)
      {
        for (int i = I; i < I + B; i += 1)
        {
          // Vectorized loop for j
          int j = J;
          for (; j <= J + B - vec_size; j += vec_size)
          {
            for (int k = K; k < K + B; k += 1)
            {
              __m256d factor = _mm256_set1_pd(A[i][k]);
              __m256d vec1 = _mm256_loadu_pd(&A[k][j]);
              __m256d vec2 = _mm256_loadu_pd(&A[i][j]);
              vec2 = _mm256_fnmadd_pd(factor, vec1, vec2);
              _mm256_storeu_pd(&A[i][j], vec2);
            }
          }

          // Remainder loop for j
          for (; j < J + B; j += 1)
          {
            for (int k = K; k < K + B; k += 1)
              A[i][j] -= A[i][k] * A[k][j];
          }
        }
      }
    }
  }
}
```

In the optimized function, we have added vectorized loops using AVX intrinsics where the loop variable only appears in the last field. This allows us to load and process multiple double values at once using AVX registers, which can significantly improve performance compared to processing them one by one. We also implemented separate loops for the vectorized part and the remainder part to handle any elements that do not fit into a full vector size.
