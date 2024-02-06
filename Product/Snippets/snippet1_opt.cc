#include <immintrin.h>
int vec_size = 4; // vec_sice = 4 since the AVX can hold 4 double values
__m256d vec1,vec2,factor; // define variables only once outside the loop
A[s] /= A[p];
factor = _mm256_set1_pd(A[s]); // load the constant only once outside the loop
int i = lower;
for (; i <= upper - vec_size; i += vec_size) // head loop (optimized using AVX)
{
  vec1 = _mm256_loadu_pd(&A[n+i]);
  vec2 = _mm256_loadu_pd(&A[m+i]);
  vec2 = _mm256_fnmadd_pd(factor,vec1,vec2);
  _mm256_storeu_pd(&A[m+i],vec2);
}
for (; i < upper; i += 1) // handle the remaining elements
  A[m+i] -= A[s] * A[n+i];