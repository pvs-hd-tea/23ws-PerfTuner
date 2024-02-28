
#include <immintrin.h>
void function(int n, double* A)
{
    __m256d vec1, vec2;  // define variables outside the loop
    int vec_size = 4;    // number of elements in AVX register, 4 for double
    for (int k = 0; k < n; k += vec_size)  // vectorized loop with step size of vec_size
    {
        for (int i = k + 1; i < n; i += 1)
        {
            // Load 4 elements at a time from arrays A into vec1 and vec2
            vec1 = _mm256_loadu_pd(&A[i*(n+1)+k]);
            vec2 = _mm256_loadu_pd(&A[k*(n+1)+k]);

            // Divide elements in vec1 by elements in vec2
            vec1 = _mm256_div_pd(vec1, vec2);

            // Update A by subtracting multiplied elements
            for (int j = k + 1; j < n; j += 1)
            {
                vec2 = _mm256_loadu_pd(&A[i*(n+1)+j]);
                __m256d mul_result = _mm256_mul_pd(vec1, vec2);
                vec2 = _mm256_sub_pd(vec2, mul_result);
                _mm256_storeu_pd(&A[i*(n+1)+j], vec2); // Store updated value in A
            }
        }
    }
}
