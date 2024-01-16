void lu_3()
{
  for (int K = 0; K < n; K += B) // K-blocks
  {
    // upper left block
    for (int k = K; k < K + B; k += 1)       // k index within block
      for (int i = k + 1; i < K + B; i += 1) // head loop for i
      {
        A[i][k] /= A[k][k];
        for (int j = k + 1; j < K + B; j += 1) // head loop for j
          A[i][j] -= A[i][k] * A[k][j];
      }

    // U blocks in first block row
    for (int J = K + B; J < n; J += B)         // block loop in j-direction
      for (int k = K; k < K + B; k += 1)       // k index within block
        for (int i = k + 1; i < K + B; i += 1) // head loop for i
          for (int j = J; j < J + B; j += 1)   // j-loop within block
            A[i][j] -= A[i][k] * A[k][j];      // consecutive access

    // L blocks in first block column
    for (int I = K + B; I < n; I += B)     // block loop in i-direction
      for (int k = K; k < K + B; k += 1)   // k index within block
        for (int i = I; i < I + B; i += 1) // i-loop within block
        {
          A[i][k] /= A[k][k];
          for (int j = k + 1; j < K + B; j += 1) // head loop for j
            A[i][j] -= A[i][k] * A[k][j];
        }

    // the rest matrix; here is the bulk of the work
    for (int I = K + B; I < n; I += B)         // block loop in i-direction
      for (int J = K + B; J < n; J += B)       // block loop in j-direction
        for (int i = I; i < I + B; i += 1)     // i-loop within block
          for (int j = J; j < J + B; j += 1)   // j-loop within block
            for (int k = K; k < K + B; k += 1) // k index within block
              A[i][j] -= A[i][k] * A[k][j];
  }
}
