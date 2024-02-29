#include <immintrin.h>
__m256d vec1,vec2;  // define variables only once outside the loop
int vec_size = 4;   // number of elements in AVX register, 4 for double
int i = 0;
for (; i <= n - vec_size; i += vec_size)     // head loop (optimized using AVX)
{
    vec1 = _mm256_loadu_pd(&a[i]);    // load necessary values in a vector
    vec2 = _mm256_loadu_pd(&b[i]);    // load necessary values in a vector
    vec2 = _mm256_sub_pd(vec1,vec2);  // perform the desired calculation
    _mm256_storeu_pd(&c[i],vec2);     // store the result into the array
}
for (; i < n; i += 1)  // handle the remaining elements
    c[i] = a[i] - b[i];  // perform the desired calculation