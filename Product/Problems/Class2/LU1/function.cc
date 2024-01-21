void function(int n, double* A)
{
  for (int k = 0; k < n; k += 1)
    for (int i = k + 1; i < n; i += 1)
    {
      A[i*(n+1)+k] /= A[k*(n+1)+k];
      for (int j = k + 1; j < n; j += 1)
        A[i*(n+1)+j] -= A[i*(n+1)+k] * A[k*(n+1)+j];
    }
}