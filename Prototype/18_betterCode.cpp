Here's the optimized version of the "Validate input" subtask using AVX2:

```cpp
#include <iostream>
#include <immintrin.h>

void validateInput_opt(const int* input, int* output, int size) {
    int i = 0;
    for (; i < size - 7; i += 8) {
        __m256i inputVec = _mm256_loadu_si256((__m256i*)(input + i));
        __m256i validMask = _mm256_cmpgt_epi32(inputVec, _mm256_set1_epi32(0));
        int validBitmask = _mm256_movemask_epi8(validMask);
        for (int j = 0; j < 8; j++) {
            if ((validBitmask >> (j*4)) & 0xf) {
                output[i + j] = input[i + j];
            } else {
                output[i + j] = 0;
            }
        }
    }
   
    for (; i < size; i++) {
        if (input[i] > 0) {
            output[i] = input[i];
        } else {
            output[i] = 0;
        }
    }
}

int main() {
    int size = 8;
    int input[] = {1, -2, 3, -4, 5, -6, 7, -8};
    int output[size];

    validateInput_opt(input, output, size);

    for (int i = 0; i < size; i++) {
        std::cout << output[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

In this optimized version, we load the input data in chunks of 8 elements using `_mm256_loadu_si256()` and compare the input vector with a vector containing all zeroes using `_mm256_cmpgt_epi32()`. We then use `_mm256_movemask_epi8()` to extract a bitmask representing the valid elements within the vector. We iterate over this bitmask and copy the valid elements to the output array. Finally, we handle any remaining elements that are not part of the 8-element chunks.

Note that the AVX2 intrinsics require the `<immintrin.h>` header file and AVX2 support from the CPU.