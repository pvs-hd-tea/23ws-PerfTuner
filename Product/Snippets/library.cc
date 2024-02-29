snippet1: // vector addition
for (int i = 0; i < n; i++)
{
    c[i] = a[i] + b[i]; // perform the desired calculation
}

snippet1_opt:
#include <immintrin.h>
__m256d vec1,vec2;  // define variables only once outside the loop
int vec_size = 4;   // number of elements in AVX register, 4 for double
int i = 0;
for (; i <= n - vec_size; i += vec_size)     // head loop (optimized using AVX)
{
    vec1 = _mm256_loadu_pd(&a[i]);   // load necessary values in a vector
    vec2 = _mm256_loadu_pd(&b[i]);   // load necessary values in a vector
    vec2 = _mm256_add_pd(vec1,vec2);  // perform the desired calculation
    _mm256_storeu_pd(&c[i],vec2);     // store the result into the array
}
for (; i < n; i += 1)  // handle the remaining elements
    c[i] = a[i] + b[i];  // perform the desired calculation

snippet2: // vector subtraction
for (int i = 0; i < n; i++)
{
    c[i] = a[i] - b[i];  // perform the desired calculation
}

snippet2_opt:
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

snippet3: // vector multiplication
for (int i = 0; i < n; i++)
{
    c[i] = a[i] * b[i];  // perform the desired calculation
}

snippet3_opt:
#include <immintrin.h>
__m256d vec1,vec2;  // define variables only once outside the loop
int vec_size = 4;   // number of elements in AVX register, 4 for double
int i = 0;
for (; i <= n - vec_size; i += vec_size)     // head loop (optimized using AVX)
{
    vec1 = _mm256_loadu_pd(&a[i]);    // load necessary values in a vector
    vec2 = _mm256_loadu_pd(&b[i]);    // load necessary values in a vector
    vec2 = _mm256_mul_pd(vec1,vec2);  // perform the desired calculation
    _mm256_storeu_pd(&c[i],vec2);     // store the result into the array
}
for (; i < n; i += 1)  // handle the remaining elements
    c[i] = a[i] * b[i];  // perform the desired calculation

snippet4: // vector division
for (int i = 0; i < n; i++)
{
    c[i] = a[i] / b[i];  // perform the desired calculation
}

snippet4_opt:
#include <immintrin.h>
__m256d vec1,vec2;  // define variables only once outside the loop
int vec_size = 4;   // number of elements in AVX register, 4 for double
int i = 0;
for (; i <= n - vec_size; i += vec_size)     // head loop (optimized using AVX)
{
    vec1 = _mm256_loadu_pd(&a[i]);    // load necessary values in a vector
    vec2 = _mm256_loadu_pd(&b[i]);    // load necessary values in a vector
    vec2 = _mm256_div_pd(vec1,vec2);  // perform the desired calculation
    _mm256_storeu_pd(&c[i],vec2);     // store the result into the array
}
for (; i < n; i += 1)  // handle the remaining elements
    c[i] = a[i] / b[i];  // perform the desired calculation

snippet5: // bitwise AND 
for (int i = 0; i < n; i++)
{
    C[i] = A[i] & B[i];            // perform the desired calculation
}

snippet5_opt:
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

snippet6: // bitwise OR
for (int i = 0; i < n; i++)
{
    C[i] = A[i] | B[i];            // perform the desired calculation
}

snippet6_opt:
#include <immintrin.h>
__m256i vec1,vec2;  // define variables only once outside the loop
int vec_size = 8;   // since the AVX register can hold 8 integers
int i = 0;
for (; i <= n - vec_size; i += vec_size)     // head loop (optimized using AVX)
{
    vec1 = _mm256_loadu_si256((__m256i*) &A[i]);   // load necessary values in a vector
    vec2 = _mm256_loadu_si256((__m256i*) &B[i]);   // load necessary values in a vector
    vec2 = _mm256_or_si256(vec1,vec2);            // perform the desired calculation
    _mm256_storeu_si256((__m256i*)&C[i],vec2);     // store the result into the array
}
for (; i < n; i += 1)  // handle the remaining elements
    C[i] = A[i] | B[i];  // perform the desired calculation

snippet7: // bitwise XOR
for (int i = 0; i < n; i++)
{
    C[i] = A[i] ^ B[i];            // perform the desired calculation
}

snippet7_opt:
#include <immintrin.h>
__m256i vec1,vec2;  // define variables only once outside the loop
int vec_size = 8;   // since the AVX register can hold 8 integers
int i = 0;
for (; i <= n - vec_size; i += vec_size)     // head loop (optimized using AVX)
{
    vec1 = _mm256_loadu_si256((__m256i*) &A[i]);   // load necessary values in a vector
    vec2 = _mm256_loadu_si256((__m256i*) &B[i]);   // load necessary values in a vector
    vec2 = _mm256_xor_si256(vec1,vec2);            // perform the desired calculation
    _mm256_storeu_si256((__m256i*)&C[i],vec2);     // store the result into the array
}
for (; i < n; i += 1)  // handle the remaining elements
    C[i] = A[i] ^ B[i];  // perform the desired calculation

snippet8: // bitwise AND NOT
for (int i = 0; i < n; i++)
{
    C[i] = A[i] & ~ B[i];            // perform the desired calculation
}

snippet8_opt:
#include <immintrin.h>
__m256i vec1,vec2;  // define variables only once outside the loop
int vec_size = 8;   // since the AVX register can hold 8 integers
int i = 0;
for (; i <= n - vec_size; i += vec_size)     // head loop (optimized using AVX)
{
    vec1 = _mm256_loadu_si256((__m256i*) &A[i]);   // load necessary values in a vector
    vec2 = _mm256_loadu_si256((__m256i*) &B[i]);   // load necessary values in a vector
    vec2 = _mm256_andnot_si256(vec1,vec2);            // perform the desired calculation
    _mm256_storeu_si256((__m256i*)&C[i],vec2);     // store the result into the array
}
for (; i < n; i += 1)  // handle the remaining elements
    C[i] = A[i] & ~ B[i];  // perform the desired calculation

snippet9: // multiplication of complex numbers
for (int i = 0; i < n; i+=2)
{
    c[i] = a[i]*b[i] - a[i+1]*b[i+1];    // calculate the real part
    c[i+1] = a[i]*b[i+1] + a[i+1]*b[i];  // calculate the imaginary part
}

snippet9_opt:
#include <immintrin.h>
int vec_size = 4;                      // vec_sice = 4 since the AVX can hold 4 double values
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

snippet10: // LU decomposition
A[s] /= A[p];
for (int i = lower; i < upper; i += 1)
  A[m+i] -= A[s] * A[n+i];   // perform the desired calculation

snippet10_opt:
#include <immintrin.h>
int vec_size = 4; // vec_sice = 4 since the AVX can hold 4 double values
__m256d vec1,vec2,factor; // define variables only once outside the loop
A[s] /= A[p];
factor = _mm256_set1_pd(A[s]); // load the constant only once outside the loop
int i = lower;
for (; i <= upper - vec_size; i += vec_size) // head loop (optimized using AVX)
{
  vec1 = _mm256_loadu_pd(&A[n+i]);  // load necessary values in a vector
  vec2 = _mm256_loadu_pd(&A[m+i]);  // load necessary values in a vector
  vec2 = _mm256_fnmadd_pd(factor,vec1,vec2); // perform the desired calculation
  _mm256_storeu_pd(&A[m+i],vec2);   // store the result into the array
}
for (; i < upper; i += 1) // handle the remaining elements
  A[m+i] -= A[s] * A[n+i];  // perform the desired calculation

snippet11: // matrix transposition
for (int i = lower1; i < lower1 + M; i++)
    for (int j = lower2; j < lower2 + M; j++)
        B[j * n + i] = A[i * n + j];  // perform desired permutation

snippet11_opt:
#include <immintrin.h>
int vec_size = 4; // vec_sice = 4 since the AVX can hold 4 double values
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
