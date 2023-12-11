Certainly! Let's consider the following subtask: Calculating the sum of two large arrays element-wise.

Assuming we have two arrays `A` and `B` of size `n`, where `n` is a multiple of `8`, the code snippet without AVX2 might look like this:

```c++
void sumArrays(const float* A, const float* B, float* result, int n) {
    for (int i = 0; i < n; i++) {
        result[i] = A[i] + B[i];
    }
}
```

To improve this subtask using AVX2, we can utilize SIMD (Single Instruction, Multiple Data) operations to parallelize the addition of elements. Here's an AVX2-optimized version of the code:

```c++
#include <immintrin.h> // Include the AVX2 header

void sumArraysAVX2(const float* A, const float* B, float* result, int n) {
    int iterations = n / 8; // Number of AVX2 iterations required
    
    // Process 8 elements at a time using AVX2
    for (int i = 0; i < iterations; i++) {
        __m256 aVec = _mm256_loadu_ps(&A[i * 8]);
        __m256 bVec = _mm256_loadu_ps(&B[i * 8]);
        
        __m256 sumVec = _mm256_add_ps(aVec, bVec);
        
        _mm256_storeu_ps(&result[i * 8], sumVec);
    }
    
    // Process the remaining elements
    for (int i = iterations * 8; i < n; i++) {
        result[i] = A[i] + B[i];
    }
}
```

In this AVX2-optimized version, we load 8 elements from each array into AVX2 registers using `_mm256_loadu_ps`, perform the addition using `_mm256_add_ps`, and store the result using `_mm256_storeu_ps`. The loop processes multiple iterations, each handling 8 elements at a time, maximizing the parallelism provided by AVX2.