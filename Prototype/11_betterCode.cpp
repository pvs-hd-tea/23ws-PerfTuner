```c++
#include <iostream>
#include <immintrin.h>

void initArrays_opt(float* arr1, float* arr2, float* arr3, float* arr4, int size) {
    // Calculate the number of elements to process per vector operation (should be a multiple of 8 for AVX2)
    int vectorSize = size / 8;
    int remainder = size % 8;
    
    // Initialize AVX2 vectors
    __m256 vec_zero = _mm256_setzero_ps();
    
    // Loop over vectorized operations
    for (int i = 0; i < vectorSize; i++) {
        // Use AVX2 to store data back into arrays
        _mm256_storeu_ps(&arr1[i * 8], vec_zero);
        _mm256_storeu_ps(&arr2[i * 8], vec_zero);
        _mm256_storeu_ps(&arr3[i * 8], vec_zero);
        _mm256_storeu_ps(&arr4[i * 8], vec_zero);
    }
    
    // Loop over the remaining elements
    for (int i = size - remainder; i < size; i++) {
        // Individual element initialization
        arr1[i] = 0.0f;
        arr2[i] = 0.0f;
        arr3[i] = 0.0f;
        arr4[i] = 0.0f;
    }
}

int main() {
    // Example usage
    int size = 20;

    float* arr1 = new float[size];
    float* arr2 = new float[size];
    float* arr3 = new float[size];
    float* arr4 = new float[size];

    initArrays_opt(arr1, arr2, arr3, arr4, size);

    for (int i = 0; i < size; i++) {
        std::cout << arr1[i] << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i < size; i++) {
        std::cout << arr2[i] << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i < size; i++) {
        std::cout << arr3[i] << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i < size; i++) {
        std::cout << arr4[i] << " ";
    }
    std::cout << std::endl;

    delete [] arr1;
    delete [] arr2;
    delete [] arr3;
    delete [] arr4;

    return 0;
}
```