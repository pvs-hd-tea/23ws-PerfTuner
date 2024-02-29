for (int i = lower1; i < lower1 + M; i++)
    for (int j = lower2; j < lower2 + M; j++)
        B[j * n + i] = A[i * n + j];  // perform desired permutation