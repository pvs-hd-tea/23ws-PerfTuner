Here is the optimized function for initializing a random array using AVX2:

```cpp
#include <immintrin.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>

// Define the AVX2 register size and float/double size
constexpr int AVX_SIZE = 8; // AVX2 register size in float
constexpr int ELEMENT_SIZE = sizeof(float); // Size of each element in bytes

// Function to initialize a random array using AVX2
void random_array_opt(float* arr, int size) {
    // Check if the array size is a multiple of the AVX2 register size
    if (size % AVX_SIZE == 0) {
        // Generate random data in batches of AVX2 register size
        for (int i = 0; i < size; i += AVX_SIZE) {
            // Generate random values and load into an AVX2 register
            __m256 rand_data = _mm256_set_ps(
                static_cast<float>(rand()) / RAND_MAX,
                static_cast<float>(rand()) / RAND_MAX,
                static_cast<float>(rand()) / RAND_MAX,
                static_cast<float>(rand()) / RAND_MAX,
                static_cast<float>(rand()) / RAND_MAX,
                static_cast<float>(rand()) / RAND_MAX,
                static_cast<float>(rand()) / RAND_MAX,
                static_cast<float>(rand()) / RAND_MAX
            );

            // Store the values from the AVX2 register into the array
            _mm256_storeu_ps(arr + i, rand_data);
        }
    } else {
        // Handle the remaining elements using scalar instructions
        int remaining_size = size % AVX_SIZE;

        // Generate random data for the remaining elements
        for (int i = 0; i < size - remaining_size; i += AVX_SIZE) {
            // Generate random values and load into an AVX2 register
            __m256 rand_data = _mm256_set_ps(
                static_cast<float>(rand()) / RAND_MAX,
                static_cast<float>(rand()) / RAND_MAX,
                static_cast<float>(rand()) / RAND_MAX,
                static_cast<float>(rand()) / RAND_MAX,
                static_cast<float>(rand()) / RAND_MAX,
                static_cast<float>(rand()) / RAND_MAX,
                static_cast<float>(rand()) / RAND_MAX,
                static_cast<float>(rand()) / RAND_MAX
            );

            // Store the values from the AVX2 register into the array
            _mm256_storeu_ps(arr + i, rand_data);
        }

        // Handle the remaining elements using scalar instructions
        for (int i = size - remaining_size; i < size; i++) {
            arr[i] = static_cast<float>(rand()) / RAND_MAX;
        }
    }
}

int main() {
    constexpr int SIZE = 30;  // Array size
    float arr[SIZE];          // Array to initialize

    // Initialize random seed
    srand(time(NULL));

    // Initialize the array using the optimized function
    random_array_opt(arr, SIZE);

    // Print the initialized array
    for (int i = 0; i < SIZE; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

This code initializes a random array using AVX2 by generating random values in batches equal to the AVX2 register size and loading them into an AVX2 register. The values are then stored from the AVX2 register directly into the array. If the array size is not a multiple of the AVX2 register size, the remaining elements are handled separately using scalar instructions.