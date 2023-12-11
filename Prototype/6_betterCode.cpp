Here is the optimized version of the function `print_status` using AVX2 intrinsics:

```cpp
#include <iostream>
#include <immintrin.h>

void print_status_opt(float* matrix, int nrows, int ncols) {
    // Check if the matrix dimensions are divisible by 8
    int remaining = nrows * ncols % 8;
    bool divisibleBy8 = (remaining == 0);

    // Create AVX2 masks for comparison
    __m256 cmp_gt_mask = _mm256_set1_ps(10.0f);
    __m256 cmp_lt_mask = _mm256_set1_ps(0.0f);

    for (int i = 0; i < nrows; i++) {
        for (int j = 0; j < ncols; j += 8) {
            // Load the next 8 elements of the matrix
            __m256 data = _mm256_loadu_ps(&matrix[i * ncols + j]);

            // Compare each element with the masks
            __m256 gt_mask = _mm256_cmp_ps(data, cmp_gt_mask, _CMP_GT_OQ);
            __m256 lt_mask = _mm256_cmp_ps(data, cmp_lt_mask, _CMP_LT_OQ);

            // Count the number of elements greater than 10 and less than 0
            int count_gt = _mm256_movemask_ps(gt_mask);
            int count_lt = _mm256_movemask_ps(lt_mask);

            // Print the status of the elements
            for (int k = 0; k < 8; k++) {
                if (j + k < ncols) {
                    std::cout << "Element " << i * ncols + j + k << ": ";
                    if (count_gt & (1 << k)) {
                        std::cout << "Greater than 10";
                    } else if (count_lt & (1 << k)) {
                        std::cout << "Less than 0";
                    } else {
                        std::cout << "Between 0 and 10";
                    }
                    std::cout << std::endl;
                }
            }
        }
    }
}
```

Note that this optimized version assumes that the input matrix is stored in row-major order. The `print_status_opt` function takes a pointer to the matrix data, as well as the number of rows and columns in the matrix. It then uses AVX2 intrinsics to process the matrix data in parallel, count the number of elements greater than 10 and less than 0, and print their statuses. The AVX2 instructions `_mm256_loadu_ps`, `_mm256_cmp_ps`, and `_mm256_movemask_ps` are used to load the matrix data, compare each element with the masks, and count the number of elements that satisfy the conditions, respectively.