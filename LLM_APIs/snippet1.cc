t /= l;
for (int i = lower; i < upper; i += 1)
  A[j][i] -= t * A[c][i];
