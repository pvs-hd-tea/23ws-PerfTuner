
#include <immintrin.h>
void function(int row, int col, double *a, double *b, double *c)
{
    // Vectorize the loop for better performance
    for (int k = 0; k < row; k+=4)
    {
        for (int i = 0; i < col; i+=4)
        {
            int c_start = k + row * i;
            int a_start = i + col * k;

            __m256d c0 = _mm256_setzero_pd();
            __m256d c1 = _mm256_setzero_pd();
            __m256d c2 = _mm256_setzero_pd();
            __m256d c3 = _mm256_setzero_pd();

            for (int j = 0; j < 4; ++j)
            {
                int a_offset = a_start + j * col;
                __m256d a_vals = _mm256_broadcast_sd(&a[a_offset]);

                int b_offset = k + j * row;
                __m256d b_vals = _mm256_loadu_pd(&b[b_offset]);

                c0 = _mm256_fmadd_pd(a_vals, b_vals, c0);

                b_offset += row;
                b_vals = _mm256_loadu_pd(&b[b_offset]);
                c1 = _mm256_fmadd_pd(a_vals, b_vals, c1);

                b_offset += row;
                b_vals = _mm256_loadu_pd(&b[b_offset]);
                c2 = _mm256_fmadd_pd(a_vals, b_vals, c2);

                b_offset += row;
                b_vals = _mm256_loadu_pd(&b[b_offset]);
                c3 = _mm256_fmadd_pd(a_vals, b_vals, c3);
            }

            _mm256_storeu_pd(&c[c_start], c0);
            _mm256_storeu_pd(&c[c_start + row], c1);
            _mm256_storeu_pd(&c[c_start + 2 * row], c2);
            _mm256_storeu_pd(&c[c_start + 3 * row], c3);
        }
    }
}
