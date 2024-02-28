
#include <immintrin.h>

void function(int n, double* a, double* b, double& c)
{
    c = 0;

    // AVX optimization
    int vec_size = 4; // AVX can handle 4 double values
    __m256d vec1, vec2, factor;
    factor = _mm256_set1_pd(c);

    for (int i = 0; i < n - vec_size + 1; i += vec_size)
    {
        vec1 = _mm256_loadu_pd(&a[i]);
        vec2 = _mm256_loadu_pd(&b[i]);
        __m256d result = _mm256_mul_pd(vec1, vec2);
        double elem_sum[4];
        _mm256_storeu_pd(elem_sum, result);

        c += elem_sum[0] + elem_sum[1] + elem_sum[2] + elem_sum[3];
    }

    // Process remaining elements
    for (int i = (n / vec_size) * vec_size; i < n; i++)
    {
        c += a[i] * b[i];
    }
}
