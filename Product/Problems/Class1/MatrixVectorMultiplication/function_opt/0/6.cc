
#include <immintrin.h>
void function(int row, int col, double *a, double *b, double *c)
{
    for (int k = 0; k < row; k+=4)
    {
        for (int i = 0; i < col; i+=4)
        {
            int c_start = k + i * row;
            int a_start = i + k * col;

            __m256d c0 = _mm256_setzero_pd();
            __m256d c1 = _mm256_setzero_pd();
            __m256d c2 = _mm256_setzero_pd();
            __m256d c3 = _mm256_setzero_pd();

            for (int ii=0; ii<4; ii++){
                int b_start = k+ii + col*i;

                __m256d a0 = _mm256_loadu_pd(&a[a_start + ii * col]);
                __m256d a1 = _mm256_loadu_pd(&a[a_start + (ii+1) * col]);
                __m256d a2 = _mm256_loadu_pd(&a[a_start + (ii+2) * col]);
                __m256d a3 = _mm256_loadu_pd(&a[a_start + (ii+3) * col]);

                __m256d t0 = _mm256_unpacklo_pd(a0, a1);
                __m256d t1 = _mm256_unpacklo_pd(a2, a3);
                __m256d t2 = _mm256_unpackhi_pd(a0, a1);
                __m256d t3 = _mm256_unpackhi_pd(a2, a3);

                __m256d b0 = _mm256_permute2f128_pd(t0, t1, 0x20);
                __m256d b1 = _mm256_permute2f128_pd(t2, t3, 0x20);
                __m256d b2 = _mm256_permute2f128_pd(t0, t1, 0x31);
                __m256d b3 = _mm256_permute2f128_pd(t2, t3, 0x31);

                c0 = _mm256_add_pd(c0, _mm256_mul_pd(b0, _mm256_loadu_pd(&b[b_start])));
                c1 = _mm256_add_pd(c1, _mm256_mul_pd(b1, _mm256_loadu_pd(&b[b_start+col])));
                c2 = _mm256_add_pd(c2, _mm256_mul_pd(b2, _mm256_loadu_pd(&b[b_start+2*col])));
                c3 = _mm256_add_pd(c3, _mm256_mul_pd(b3, _mm256_loadu_pd(&b[b_start+3*col]));
            }

            _mm256_stream_pd(&c[c_start], c0);
            _mm256_stream_pd(&c[c_start+row], c1);
            _mm256_stream_pd(&c[c_start+2*row], c2);
            _mm256_stream_pd(&c[c_start+3*row], c3);
        }
    }
}
