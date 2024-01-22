Here's the optimized version of the relevant subtasks in the original function, using the explanation provided earlier:

```cpp
void function(int row1, int col1, int col2, double *a, double *b, double *c)
{
    int vec_size = 4;
    __m256d vec1, vec2, factor;
    
    for (int k = 0; k < row1; k++)
    {
        for (int j = 0; j < col2; j++)
        {
            int i = 0;

            // Head loop optimized using AVX
            for (; i <= col1 - vec_size; i += vec_size)
            {
                vec1 = _mm256_loadu_pd(&a[k * col1 + i]);
                vec2 = _mm256_loadu_pd(&b[i * col2 + j]);
                vec2 = _mm256_fnmadd_pd(vec1, vec2, _mm256_loadu_pd(&c[k * row1 + j]));
                _mm256_storeu_pd(&c[k * row1 + j], vec2);
            }
            
            // Handle the remaining elements
            for (; i < col1; i++)
            {
                c[k * row1 + j] += a[k * col1 + i] * b[i * col2 + j];
            }
        }
    }
}
```

In this optimized version, we leverage AVX intrinsics to perform parallel computations on `vec_size` elements at a time for the inner loop. The AVX instructions `_mm256_loadu_pd()`, `_mm256_fnmadd_pd()`, and `_mm256_storeu_pd()` are used to load, perform the calculation, and store the modified elements back into the `c` array. This allows us to take advantage of SIMD operations to improve the performance of the code.