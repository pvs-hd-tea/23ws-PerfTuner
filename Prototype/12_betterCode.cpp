#include <iostream>
#include <immintrin.h>

void InitializeVariables_opt(float* data, int n) {
    const int simd_width = 8;
    const int simd_size = simd_width * sizeof(float);
    const int simd_elements = simd_width / sizeof(float);
    
    // Allocate memory aligned to AVX2 SIMD width
    float* aligned_data = (float*) _mm_malloc(n * sizeof(float), simd_size);
    
    // Initialize AVX2 registers with initial values
    __m256 initial_values = _mm256_set1_ps(0.0);
    
    // Initialize variables in parallel
    for (int i = 0; i < n - simd_elements + 1; i += simd_elements) {
        _mm256_store_ps(aligned_data + i, initial_values);
    }
    
    // Handle remaining variables separately
    for (int i = (n / simd_elements) * simd_elements; i < n; ++i) {
        aligned_data[i] = 0.0;
    }
    
    // Copy aligned data to the original array
    for (int i = 0; i < n; ++i) {
        data[i] = aligned_data[i];
    }
    
    // Free the aligned memory
    _mm_free(aligned_data);
}

int main() {
    const int n = 1000;
    float* data = new float[n];
    
    InitializeVariables_opt(data, n);
    
    // Print the initialized values
    for (int i = 0; i < n; ++i) {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
    
    delete[] data;
    
    return 0;
}