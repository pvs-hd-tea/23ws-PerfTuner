#include <immintrin.h>
void function(int n, int *A, int *B, int *C)
{
  for (int i = 0; i < n; i++)
    C[i] = A[i] & B[i];            // perform the desired calculation
}