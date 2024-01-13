k /= l;
for (int i = lower; i < upper; i += 1)
  A[j][i] -= k * A[c][i];
