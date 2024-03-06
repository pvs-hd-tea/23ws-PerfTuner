#include <immintrin.h>
void function(int n, int *A, int *B, int *C, int *D, int *E)
{
  for (int i = 0; i < n; i++)
    E[i] = (A[i] & B[i]) | (C[i] ^ D[i]);            // perform the desired calculation
}