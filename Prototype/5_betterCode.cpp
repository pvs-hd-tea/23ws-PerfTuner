Here is the optimized function to initialize a matrix using AVX2 instructions:

```cpp
#include <immintrin.h>

void InitializeMatrix_avx2(float* matrix, int rows, int cols, float value) {
  // Align the memory (assuming 32-byte alignment)
  float* alignedMatrix = (float*)_mm_malloc(rows * cols * sizeof(float), 32);

  // Divide the initialization process
  const int avxSize = 8;
  const int alignSize = avxSize * (cols / avxSize);
  
  // Load constants into vector registers
  __m256 valueVector = _mm256_set1_ps(value);

  // Set up loop counters
  int i, j;
  
  // Perform parallel initialization
  for (i = 0; i < rows; ++i) {
    // Initialize aligned memory
    for (j = 0; j < alignSize; j += avxSize) {
      _mm256_store_ps(alignedMatrix + i * cols + j, valueVector);
    }

    // Handle remaining elements
    for (j = alignSize; j < cols; ++j) {
      alignedMatrix[i * cols + j] = value;
    }
  }
  
  // Store the initialized matrix (assuming alignedMatrix is aligned)
  for (i = 0; i < rows; ++i) {
    for (j = 0; j < cols; ++j) {
      matrix[i * cols + j] = alignedMatrix[i * cols + j];
    }
  }

  // Free the aligned memory
  _mm_free(alignedMatrix);
}
```

To execute the provided function, you can use the following `main` function:

```cpp
#include <iostream>

int main() {
  int rows = 4;
  int cols = 8;
  float value = 5.0f;

  float* matrix = new float[rows * cols];

  // Call the optimized function
  InitializeMatrix_avx2(matrix, rows, cols, value);

  // Print the initialized matrix
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      std::cout << matrix[i * cols + j] << " ";
    }
    std::cout << std::endl;
  }

  delete[] matrix;

  return 0;
}
```

Note that you need to include the `<immintrin.h>` header file and compile with the appropriate AVX2 flags (`-mavx2` for GCC/Clang or `/arch:AVX2` for MSVC) to enable AVX2 instruction set support. Also, make sure to properly handle memory allocation and deallocation for the `matrix` array.