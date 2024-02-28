
#include <immintrin.h>
void function(int n, double a, double *b, double *c)
{
    int vec_size = 4; // Set vector size to 4 for AVX
    __m256d avec, bvec, scalar; // Declare AVX variables
    scalar = _mm256_set1_pd(a); // Broadcast 'a' to AVX register
    int i = 0;
    for (; i <= n - vec_size; i += vec_size)
    {
        bvec = _mm256_loadu_pd(&b[i]); // Load 4 elements of array 'b' into AVX register
        avec = _mm256_mul_pd(scalar, bvec); // Multiply 'a' with 'b'
        _mm256_storeu_pd(&c[i], avec); // Store the result in array 'c'
    }
    for (; i < n; i++)
    {
        c[i] = a * b[i]; // Process remaining elements
    }
}
