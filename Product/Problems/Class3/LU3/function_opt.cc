
#include <immintrin.h>

void function(int n, int B, double* A) {
  for (int K = 0; K < n; K += B) {
    // upper left block
    for (int k = K; k < K + B; k += 1) {
      for (int i = k + 1; i < K + B; i += 1) {
        A[i * (n + 1) + k] /= A[k * (n + 1) + k];

        // Use AVX intrinsics for optimized memory access
        __m256d v1 = _mm256_loadu_pd(&A[i * (n + 1) + k + 1]);
        __m256d v2 = _mm256_broadcast_sd(&A[k * (n + 1) + k]);
        __m256d result = _mm256_mul_pd(v1, v2);
        __m256d v3 = _mm256_loadu_pd(&A[i * (n + 1) + k + 1]);
        __m256d v4 = _mm256_loadu_pd(&A[k * (n + 1) + k + 1]);
        result = _mm256_sub_pd(v3, _mm256_mul_pd(result, v4));

        _mm256_storeu_pd(&A[i * (n + 1) + k + 1], result);
      }
    }

    // U blocks in first block row
    for (int J = K + B; J < n; J += B) {
      for (int k = K; k < K + B; k += 1) {
        for (int i = k + 1; i < K + B; i += 1) {
          for (int j = J; j < J + B; j += 1)
            A[i * (n + 1) + j] -= A[i * (n + 1) + k] * A[k * (n + 1) + j];
        }
      }
    }

    // L blocks in first block column
    for (int I = K + B; I < n; I += B) {
      for (int k = K; k < K + B; k += 1) {
        for (int i = I; i < I + B; i += 1) {
          A[i * (n + 1) + k] /= A[k * (n + 1) + k];

          // Use AVX intrinsics for optimized memory access
          __m256d v1 = _mm256_loadu_pd(&A[i * (n + 1) + k + 1]);
          __m256d v2 = _mm256_broadcast_sd(&A[k * (n + 1) + k]);
          __m256d result = _mm256_mul_pd(v1, v2);
          __m256d v3 = _mm256_loadu_pd(&A[i * (n + 1) + k + 1]);
          __m256d v4 = _mm256_loadu_pd(&A[k * (n + 1) + k + 1]);
          result = _mm256_sub_pd(v3, _mm256_mul_pd(result, v4));

          _mm256_storeu_pd(&A[i * (n + 1) + k + 1], result);
        }
      }
    }

    // the rest matrix
    for (int I = K + B; I < n; I += B) {
      for (int J = K + B; J < n; J += B) {
        for (int i = I; i < I + B; i += 1) {
          for (int j = J; j < J + B; j += 1) {
            for (int k = K; k < K + B; k += 1)
              A[i * (n + 1) + j] -= A[i * (n + 1) + k] * A[k * (n + 1) + j];
          }
        }
      }
    }
  }
}
