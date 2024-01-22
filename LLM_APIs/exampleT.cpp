const int n = 48;  // n should be a multiple of B
const int B = 12;  // B should be a multiple of 4
double A[n][n+1];  // matrix to be solved
double x[n];       // final results
int randNum[n][n]; // random numbers for initializing the matrix

// Version 1: standard kij version
void lu_1()
{
  for (int k = 0; k < n; k += 1)             // row k is (normalized) subtracted from row i
    for (int i = k + 1; i < n; i += 1)       // columns are denoted by j
    {
      A[i][k] /= A[k][k];                    // (normalization) factor
      for (int j = k + 1; j < n; j += 1)     // only elements with j>k are needed later on
        A[i][j] -= A[i][k] * A[k][j];        // actual calculation
    }
}