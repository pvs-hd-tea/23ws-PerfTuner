#include <immintrin.h>
int vec_size = 4; // vec_size = 4 since the AVX can hold 4 double values
int A_start, B_start;
__m256d a0,a1,a2,a3,t0,t1,t2,t3,b0,b1,b2,b3; // define variables only once outside the loop
for (int i = lower1; i < lower1 + M; i+= vec_size){
        for (int j = lower2; j < lower2 + M; j+= vec_size){
          A_start = i * n + j;
          B_start = j * n + i;

          // load necessary values in vectors
          a0 = _mm256_loadu_pd(&A[A_start]);     
          a1 = _mm256_loadu_pd(&A[A_start+n]);    
          a2 = _mm256_loadu_pd(&A[A_start+2*n]); 
          a3 = _mm256_loadu_pd(&A[A_start+3*n]); 

          // Unpack and interleave values
          t0 = _mm256_unpacklo_pd(a0, a1);  
          t1 = _mm256_unpacklo_pd(a2, a3);  
          t2 = _mm256_unpackhi_pd(a0, a1);  
          t3 = _mm256_unpackhi_pd(a2, a3);  

          // perform desired permutation  
          b0 = _mm256_permute2f128_pd(t0, t1, 0x20);  
          b1 = _mm256_permute2f128_pd(t2, t3, 0x20);  
          b2 = _mm256_permute2f128_pd(t0, t1, 0x31);  
          b3 = _mm256_permute2f128_pd(t2, t3, 0x31);  

          // Use non-temporal stores
          _mm256_stream_pd (&B[B_start], b0);
          _mm256_stream_pd (&B[B_start+n], b1);
          _mm256_stream_pd (&B[B_start+2*n], b2);
          _mm256_stream_pd (&B[B_start+3*n], b3);
        }
}