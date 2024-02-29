for (int ii = i; ii < i + M; ii++)
    for (int jj = j; jj < j + M; jj++)
        B[jj * n + ii] = A[ii * n + jj];  // perform desired permutation