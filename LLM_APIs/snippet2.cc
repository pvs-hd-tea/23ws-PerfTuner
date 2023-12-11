int vec_size = 4; // vec_sice = 4 since the AVX can hold 4 double values
__m256 vec1,vec2,factor;
for (int i = lower; i < upper; i += 1)
{
   factor = _mm256_set1_pd(k);
   int j = lower;
   for (; j <= upper - vec_size; j += vec_size)  <--
   {
     vec1 = _mm256_loadu_pd(&A[c][j]);
     vec2 = _mm256_loadu_pd(&A[i][j]);
     vec2 = _mm256_fnmadd_pd(factor,vec1,vec2);
     _mm256_storeu_pd(&A[i][j],vec2);
   }
   for (; j < upper; j += 1)                     <--
     A[i][j] -= k * A[c][j];
}
