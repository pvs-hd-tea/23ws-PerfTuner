
#include <immintrin.h>
void function(int n, double *a, double *b, double *c)
{
    __m256d vec1, vec2;  // define AVX variables outside the loop
    int vec_size = 4;    // number of elements in AVX register, 4 for double

    for (int i = 0; i <= n - vec_size; i += vec_size)  // head loop (optimized using AVX)
    {
        vec1 = _mm256_loadu_pd(&a[i]);
        vec2 = _mm256_loadu_pd(&b[i]);
        vec2 = _mm256_add_pd(vec1, vec2);
        _mm256_storeu_pd(&c[i], vec2);
    }

    for (int i = n - (n % vec_size); i < n; i++)  // handle the remaining elements
    {
        c[i] = a[i] + b[i];
    }
}