Here's the optimized function sumArraysAVX2():

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

And here's the main function to test the optimized function:

```c++
#include <iostream>
#include <chrono>

int main() {
    int n = 1000000; // Number of elements
    
    // Arrays A and B
    float* A = new float[n];
    float* B = new float[n];
    
    // Initialize A and B with random values
    for (int i = 0; i < n; i++) {
        A[i] = static_cast<float>(rand()) / RAND_MAX;
        B[i] = static_cast<float>(rand()) / RAND_MAX;
    }
    
    // Result array
    float* result = new float[n];
    
    // Measure the execution time before vectorization
    auto start = std::chrono::high_resolution_clock::now();
    sumArrays(A, B, result, n);
    auto end = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double> duration = end - start;
    std::cout << "Execution time (without AVX2): " << duration.count() << "s" << std::endl;
    
    // Measure the execution time after vectorization
    start = std::chrono::high_resolution_clock::now();
    sumArraysAVX2(A, B, result, n);
    end = std::chrono::high_resolution_clock::now();
    
    duration = end - start;
    std::cout << "Execution time (with AVX2): " << duration.count() << "s" << std::endl;
    
    // Cleanup
    delete[] A;
    delete[] B;
    delete[] result;
    
    return 0;
}
```

You can test this code with the same input data as in the original C++ code and compare the execution times before and after vectorization.