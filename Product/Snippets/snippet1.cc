A[s] /= A[p];
for (int i = lower; i < upper; i += 1)
  A[m+i] -= A[s] * A[n+i];        