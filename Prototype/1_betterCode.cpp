Here is an optimized version of the subtask using AVX2 intrinsics:

```cpp
#include <immintrin.h>

// AVX2 optimized function
void subtask_opt(float *a, float *b, float *c, int n) {
    // Load 8 values from arrays a and b into AVX2 registers
    __m256 vec_a = _mm256_loadu_ps(a);
    __m256 vec_b = _mm256_loadu_ps(b);

    // Compute the dot product of vec_a and vec_b
    __m256 dot_product = _mm256_dp_ps(vec_a, vec_b, 255);

    // Store the result back to array c
    _mm256_storeu_ps(c, dot_product);
}

int main() {
    // Input data
    int n = 8;
    float a[n] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
    float b[n] = {2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
    float c[n] = {0.0};

    // Call the optimized function
    subtask_opt(a, b, c, n);

    // Print the result
    for (int i = 0; i < n; i++) {
        printf("%f ", c[i]);
    }
    printf("\n");

    return 0;
}
```

In this optimized version, we use the `_mm256_loadu_ps` and `_mm256_storeu_ps` intrinsic functions to load and store the data, respectively. The `_mm256_dp_ps` intrinsic function is used to compute the dot product of two vectors. The AVX2 registers are 256 bits (32 bytes) wide, so loading and storing data in 32-byte aligned chunks maximizes the performance.

The provided `main` function demonstrates the usage of the optimized `subtask_opt` function. It initializes the input arrays `a` and `b`, and the output array `c`. Then, it calls the `subtask_opt` function with the input arrays and the size of the arrays. Finally, it prints the result array `c` for verification.

Please note that the code provided is a simplified example to demonstrate the usage of AVX2 intrinsics for the given subtask. Depending on the actual requirements and context of your code, further optimizations and adjustments may be necessary.