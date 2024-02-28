
#include <immintrin.h>
void function(int n, double *a, double *b, double &c)
{
    __m256d vec1, vec2;      // Define AVX registers
    int vec_size = 4;        // Number of elements in AVX register (4 for double)
    
    int i = 0;
    for (; i <= n - vec_size; i += vec_size)    // Loop over elements in a and b
    {
        vec1 = _mm256_loadu_pd(&a[i]);          // Load elements from array a
        vec2 = _mm256_loadu_pd(&b[i]);          // Load elements from array b
        vec2 = _mm256_sub_pd(vec1, vec2);       // Subtract elements in vec1 from vec2
        _mm256_storeu_pd(&c[i], vec2);          // Store the result in c
    }

    // Handle remaining elements
    for (; i < n; i++)
    {
        c[i] = a[i] - b[i];
    }
}
