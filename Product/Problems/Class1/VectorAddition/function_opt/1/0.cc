
#include <immintrin.h>
void function(int n, double *a, double *b, double *c)
{
    __m256d vec1, vec2;  // Define variables only once outside the loop
    int vec_size = 4;   // Number of elements in AVX register, 4 for double
    int i = 0;
    
    for (; i <= n - vec_size; i += vec_size)     // Head loop (optimized using AVX)
    {
        vec1 = _mm256_loadu_pd(&a[i]); // Load 4 elements from array 'a'
        vec2 = _mm256_loadu_pd(&b[i]); // Load 4 elements from array 'b'
        vec2 = _mm256_sub_pd(vec1, vec2); // Subtract corresponding elements
        _mm256_storeu_pd(&c[i], vec2); // Store the result in array 'c'
    }
    
    for (; i < n; i += 1)  // Handle the remaining elements
    {
        c[i] = a[i] - b[i]; // Perform subtraction for single elements
    }
}
