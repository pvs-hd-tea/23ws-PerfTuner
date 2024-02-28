
#include <immintrin.h>
void function(int n, int B, double* A)
{
  __m256d vec1, vec2;
  int vec_size = 4;

  for (int K = 0; K < n; K += B) // K-blocks
  {
    for (int k = K; k < K + B; k += 1)       // k index within block
    {
      for (int i = k + 1; i < K + B; i += 1) // head loop for i
      {
        vec1 = _mm256_set1_pd(A[k * (n + 1) + k]);
        vec2 = _mm256_loadu_pd(&A[i * (n + 1) + k]);
        vec2 = _mm256_div_pd(vec2, vec1);
        _mm256_storeu_pd(&A[i * (n + 1) + k], vec2);

        for (int j = k + 1; j < K + B; j += 1) // head loop for j
        {
          vec1 = _mm256_set1_pd(A[i * (n + 1) + k]);
          vec2 = _mm256_loadu_pd(&A[k * (n + 1) + j]);
          vec2 = _mm256_mul_pd(vec2, _mm256_set1_pd(A[k * (n + 1) + j]));
          _mm256_storeu_pd(&A[i * (n + 1) + j], vec2);
        }
      }
    }

    for (int J = K + B; J < n; J += B)         // block loop in j-direction
    {
      for (int k = K; k < K + B; k += 1)       // k index within block
      {
        for (int i = k + 1; i < K + B; i += 1) // head loop for i
        {
          for (int j = J; j < J + B; j += 1)   // j-loop within block
          {
            vec1 = _mm256_set1_pd(A[i * (n + 1) + k]);
            vec2 = _mm256_loadu_pd(&A[k * (n + 1) + j]);
            vec2 = _mm256_mul_pd(vec2, _mm256_set1_pd(A[i * (n + 1) + k]));
            _mm256_storeu_pd(&A[i * (n + 1) + j], _mm256_sub_pd(vec2, _mm256_loadu_pd(&A[i * (n + 1) + j]));
          }
        }
      }
    }

    for (int I = K + B; I < n; I += B)     // block loop in i-direction
    {
      for (int k = K; k < K + B; k += 1)   // k index within block
      {
        for (int i = I; i < I + B; i += 1) // i-loop within block
        {
          vec1 = _mm256_set1_pd(A[k * (n + 1) + k]);
          vec2 = _mm256_loadu_pd(&A[i * (n + 1) + k]);
          vec2 = _mm256_div_pd(vec2, vec1);
          _mm256_storeu_pd(&A[i * (n + 1) + k], vec2);

          for (int j = k + 1; j < K + B; j += 1) // head loop for j
          {
            vec1 = _mm256_set1_pd(A[i * (n + 1) + k]);
            vec2 = _mm256_loadu_pd(&A[k * (n + 1) + j]);
            vec2 = _mm256_mul_pd(vec2, _mm256_set1_pd(A[i * (n + 1) + k]));
            _mm256_storeu_pd(&A[i * (n + 1) + j], _mm256_sub_pd(vec2, _mm256_loadu_pd(&A[i * (n + 1) + j]));
          }
        }
      }
    }

    for (int I = K + B; I < n; I += B)         // block loop in i-direction
    {
      for (int J = K + B; J < n; J += B)       // block loop in j-direction
      {
        for (int i = I; i < I + B; i += 1)     // i-loop within block
        {
          for (int j = J; j < J + B; j += 1)   // j-loop within block
          {
            for (int k = K; k < K + B; k += 1) // k index within block
            {
              vec1 = _mm256_loadu_pd(&A[i * (n + 1) + k]);
              vec2 = _mm256_loadu_pd(&A[k * (n + 1) + j]);
              vec1 = _mm256_mul_pd(vec1, _mm256_set1_pd(A[k * (n + 1) + j]));
              vec2 = _mm256_sub_pd(_mm256_loadu_pd(&A[i * (n + 1) + j]), vec1);
              _mm256_storeu_pd(&A[i * (n + 1) + j], vec2);
            }
          }
        }
      }
    }
  }
}
