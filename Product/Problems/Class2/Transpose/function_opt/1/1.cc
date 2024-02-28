
#include <immintrin.h>

void function(int n, int M, double *A, double *B)
{
    __m256d vec1, vec2;
    int vec_size = 4;
    int i = 0;

    for (; i <= n - vec_size; i += vec_size) // vectorized loop using AVX
    {
        for (int j = 0; j < n; j++)
        {
            vec1 = _mm256_loadu_pd(&A[i * n + j]);
            vec2 = _mm256_loadu_pd(&B[j * n + i]);
            vec2 = _mm256_add_pd(vec1, vec2);
            _mm256_storeu_pd(&B[j * n + i], vec2);
        }
    }

    for (; i < n; i++) // scalar loop for remaining elements
    {
        for (int j = 0; j < n; j++)
        {
            B[j * n + i] = A[i * n + j];
        }
    }
}
