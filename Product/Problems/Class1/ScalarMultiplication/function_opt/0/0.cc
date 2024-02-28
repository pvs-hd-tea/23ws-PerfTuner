
#include <immintrin.h>
void function(int n, double a, double *b, double *c)
{
    int vec_size = 4;
    __m256d vec_a = _mm256_set1_pd(a); // Load scalar 'a' into AVX vector
    int i = 0;
    for (; i <= n - vec_size; i += vec_size)
    {
        __m256d vec_b = _mm256_loadu_pd(&b[i]); // Load 4 double values from array b starting at index i
        __m256d vec_result = _mm256_mul_pd(vec_a, vec_b); // Multiply 'a' with the elements of vec_b
        _mm256_storeu_pd(&c[i], vec_result); // Store the result in array c starting at index i
    }
    for (; i < n; i++)
    {
        c[i] = a * b[i]; // Handle the remaining elements individually
    }
}
