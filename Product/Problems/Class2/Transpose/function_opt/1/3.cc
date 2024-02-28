
#include <immintrin.h>
void function(int n, int M, double *A, double *B)
{
    __m256d vec1, vec2;  // define AVX variables
    int vec_size = 4;    // number of elements in AVX register
    for (int i = 0; i < n; i += vec_size)   // optimized loop using AVX
    {
        for (int j = 0; j < n; j++)
        {
            vec1 = _mm256_loadu_pd(&A[i * n + j]);
            vec2 = _mm256_loadu_pd(&B[j * n + i]);
            vec2 = _mm256_add_pd(vec1, vec2);
            _mm256_storeu_pd(&B[j * n + i], vec2);
        }
    }
}
