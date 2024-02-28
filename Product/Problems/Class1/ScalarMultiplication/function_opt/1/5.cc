
#include <immintrin.h>
void function(int n, double a, double *b, double *c)
{
    int vectorSize = 4;
    for (int i = 0; i < n; i += vectorSize)
    {
        int remaining = n - i;
        int elements = remaining < vectorSize ? remaining : vectorSize;

        __m256d scalar = _mm256_set1_pd(a);
        for (int j = 0; j < elements; j++)
        {
            int idx = i + j;
            __m256d vec_b = _mm256_loadu_pd(&b[idx]); // Load vector of 4 double precision values from b
            __m256d result = _mm256_mul_pd(scalar, vec_b); // Multiply the vector by the scalar
            _mm256_storeu_pd(&c[idx], result); // Store the result vector in c
        }
    }
}
