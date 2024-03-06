#include <immintrin.h>
void function(int n, int M, double *A, double *B)
{
  for (int i = 0; i < n; i += M)   // split i loop in blocks of size M
    for (int j = 0; j < n; j += M) // split j loop in blocks of size M
      for (int ii = i; ii < i + M; ii++)
        for (int jj = j; jj < j + M; jj++)
          B[jj * n + ii] = A[ii * n + jj];
}