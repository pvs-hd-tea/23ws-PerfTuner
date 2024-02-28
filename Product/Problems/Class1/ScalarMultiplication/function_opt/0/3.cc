
#include <immintrin.h>
void function(int n, double a, double *b, double *c)
{
    __m256d vec_a = _mm256_set1_pd(a); // Load the scalar 'a' into AVX register
    int vec_size = 4; // AVX register can hold 4 double values
    int i = 0;
    for (; i <= n - vec_size; i += vec_size) // Process 4 elements at a time using AVX
    {
        __m256d vec_b = _mm256_loadu_pd(&b[i]); // Load 4 elements of 'b' array into AVX register
        __m256d vec_result = _mm256_mul_pd(vec_a, vec_b); // Multiply 'a' with 'b' elements
        _mm256_storeu_pd(&c[i], vec_result); // Store the result in 'c' array
    }

    for (; i < n; i++) // Handle remaining elements
    {
        c[i] = a * b[i]; // Perform scalar multiplication for the remaining elements
    }
}
