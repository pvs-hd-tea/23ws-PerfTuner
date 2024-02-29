#include <immintrin.h>
__m256i vec1,vec2;  // define variables only once outside the loop
int vec_size = 8;   // since the AVX register can hold 8 integers
int i = 0;
for (; i <= n - vec_size; i += vec_size)     // head loop (optimized using AVX)
{
    vec1 = _mm256_loadu_si256((__m256i*) &A[i]);   // load necessary values in a vector
    vec2 = _mm256_loadu_si256((__m256i*) &B[i]);   // load necessary values in a vector
    vec2 = _mm256_and_si256(vec1,vec2);            // perform the desired calculation
    _mm256_storeu_si256((__m256i*)&C[i],vec2);     // store the result into the array
}
for (; i < n; i += 1)  // handle the remaining elements
    C[i] = A[i] & B[i];  // perform the desired calculation
