#include <immintrin.h>
int vec_size = 4;                      // vec_size = 4 since the AVX can hold 4 double values
__m256d vec1, vec2, ayx, byy, p1, bxx; // define variables only once outside the loop
int i = 0;
for (; i <= n - vec_size; i += vec_size) // head loop (optimized using AVX)
{
    // load necessary values to vectors
    vec1 = _mm256_loadu_pd(&a[i]);
    vec2 = _mm256_loadu_pd(&b[i]);
    // prepare temporary vectors
    ayx = _mm256_permute_pd(vec1, _MM_SHUFFLE(0, 0, 1, 1));
    byy = _mm256_permute_pd(vec2, _MM_SHUFFLE(0, 0, 3, 3));
    bxx = _mm256_permute_pd(vec2, _MM_SHUFFLE(0, 0, 0, 0));
    // calculate temporary product
    p1 = _mm256_mul_pd(ayx, byy);
    // calculate final product and store the result into the array
    _mm256_storeu_pd(&c[i], _mm256_fmaddsub_pd(vec1, bxx, p1));
}
for (; i < n; i += 2) // handle the remaining elements
{
    c[i] = a[i]*b[i] - a[i+1]*b[i+1];    // calculate the real part
    c[i+1] = a[i]*b[i+1] + a[i+1]*b[i];  // calculate the imaginary part
}