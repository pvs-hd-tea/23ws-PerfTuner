Based on the explanation provided for the snippet, here's how we can optimize the relevant subtasks of the original function using AVX intrinsics:

1. Split the i loop in blocks of size M:
   - Optimize: Instead of splitting the loop, we can process multiple elements within each iteration of the loop using AVX intrinsics. This will effectively achieve the same goal of splitting the loop into blocks.

2. Iterate over the i blocks:
   - Optimize: Instead of iterating over the i blocks, we will use a regular loop with an increment of `vec_size` (e.g., 4) to process multiple elements in parallel using AVX intrinsics.

3. Iterate over the elements within each i block:
   - Optimize: Inside the AVX loop, we will load and store multiple elements at a time using AVX intrinsic functions. This will eliminate the need for individual element-wise operations within the loop and improve performance.

4. Copy the element from A to B, while swapping the indices:
   - No specific optimization related to AVX is required for this task.

Here's the optimized code:

```cpp
void function(int n, int M, double *A, double *B)
{
  int vec_size = 4;
  __m256d vec1, vec2;
  
  for (int i = 0; i < n; i += vec_size) // Process multiple elements in parallel
  {
    for (int j = 0; j < n; j += M) // Split j loop in blocks of size M
    {
      for (int ii = i; ii < i + vec_size; ii++) // Iterate over the AVX elements
      {
        for (int jj = j; jj < j + M; jj++)
        {
          vec1 = _mm256_loadu_pd(&A[ii * n + jj]); // Load elements from A
          vec2 = _mm256_loadu_pd(&B[jj * n + ii]); // Load elements from B
          _mm256_storeu_pd(&B[jj * n + ii], vec1); // Store elements from A into B
          _mm256_storeu_pd(&A[ii * n + jj], vec2); // Store elements from B into A
        }
      }
    }
  }
}
```

This optimized code utilizes AVX intrinsics to process multiple elements in parallel, resulting in improved performance compared to the original code.