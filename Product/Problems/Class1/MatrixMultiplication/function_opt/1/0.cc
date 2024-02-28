
#include <immintrin.h>

void function(int row1, int col1, int col2, double *a, double *b, double *c)
{
    int vec_size = 4; // number of elements in AVX register (double)
    __m256d vec1, vec2, vec3;

    for (int k = 0; k < row1; k++)
    {
        for (int j = 0; j < col2; j += vec_size)
        {
            vec3 = _mm256_setzero_pd(); // Initialize as zero

            for (int i = 0; i < col1; i++)
            {
                vec1 = _mm256_loadu_pd(&a[k * col1 + i]); // Load elements of a
                vec2 = _mm256_loadu_pd(&b[i * col2 + j]); // Load elements of b
                vec2 = _mm256_mul_pd(vec1, vec2);          // Multiply a and b
                vec3 = _mm256_add_pd(vec3, vec2);          // Add the result to vec3
            }
            _mm256_storeu_pd(&c[k * row1 + j], vec3);     // Store the final result in c
        }
    }
}
