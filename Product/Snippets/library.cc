snippet1: // LU decomposition
for (int i = lower; i < upper; i += 1)
  A[j][i] -= c * A[c][i];

snippet1_opt:
int vec_size = 4; // vec_sice = 4 since the AVX can hold 4 double values
__m256d vec1,vec2,factor; // define variables only once outside the loop
factor = _mm256_set1_pd(A[j][c]); // load the constant only once outside the loop
int i = lower;
for (; i <= upper - vec_size; i += vec_size) // head loop (optimized using AVX)
{
  vec1 = _mm256_loadu_pd(&A[c][i]);
  vec2 = _mm256_loadu_pd(&A[j][i]);
  vec2 = _mm256_fnmadd_pd(factor,vec1,vec2);
  _mm256_storeu_pd(&A[j][i],vec2);
}
for (; i < upper; i += 1) // Handle the remaining elements
  A[j][i] -= k * A[c][i];

snippet2: // matrix transposition
for (int ii = i; ii < i + M; ii++)
    for (int jj = j; jj < j + M; jj++)
        B[jj * n + ii] = A[ii * n + jj];

snippet2_opt:
for (int ii=0; ii<8; ii+=4){
        for (int jj=0; jj<8; jj+=4){
          int b_start = i+ii + n*(j+jj);
          int a_start = j+jj + n*(i+ii);

          __m256d a0 = _mm256_loadu_pd(&a[a_start]);
          __m256d a1 = _mm256_loadu_pd(&a[a_start+n]);
          __m256d a2 = _mm256_loadu_pd(&a[a_start+2*n]);
          __m256d a3 = _mm256_loadu_pd(&a[a_start+3*n]);

          __m256d t0 = _mm256_unpacklo_pd(a0, a1);
          __m256d t1 = _mm256_unpacklo_pd(a2, a3);
          __m256d t2 = _mm256_unpackhi_pd(a0, a1);
          __m256d t3 = _mm256_unpackhi_pd(a2, a3);

          __m256d b0 = _mm256_permute2f128_pd(t0, t1, 0x20);
          __m256d b1 = _mm256_permute2f128_pd(t2, t3, 0x20);
          __m256d b2 = _mm256_permute2f128_pd(t0, t1, 0x31);
          __m256d b3 = _mm256_permute2f128_pd(t2, t3, 0x31);

          // Use non-temporal stores
          _mm256_stream_pd (&b[b_start], b0);
          _mm256_stream_pd (&b[b_start+n], b1);
          _mm256_stream_pd (&b[b_start+2*n], b2);
          _mm256_stream_pd (&b[b_start+3*n], b3);
        }
      }