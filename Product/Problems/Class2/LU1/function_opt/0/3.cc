
#include <immintrin.h>
void function(int n, double* A)
{
    __m256d vec1, vec2;  // Define AVX variables outside the loops
    int vec_size = 4;     // Number of elements in AVX register
    int i,k,j;
    for (k = 0; k < n; k += 1)
        for (i = k + 1; i < n; i += vec_size)
        {
            vec1 = _mm256_loadu_pd(&A[i*(n+1)+k]); // Load 4 elements from A[i*(n+1)+k] into vec1
            vec1 = _mm256_div_pd(vec1, _mm256_broadcast_sd(&A[k*(n+1) + k])); // Divide vec1 by A[k*(n+1)+k]
            for (j = k + 1; j < n; j += 1)
            {
                vec2 = _mm256_loadu_pd(&A[i*(n+1) + j]); // Load 4 elements from A[i*(n+1)+j] into vec2
                vec2 = _mm256_sub_pd(vec2, _mm256_mul_pd(vec1, _mm256_loadu_pd(&A[k*(n+1) + j]))); // Subtract vec1 * A[k*(n+1)+j] from vec2
                _mm256_storeu_pd(&A[i*(n+1) + j], vec2); // Store the result back into A[i*(n+1)+j]
            }
        }
}
