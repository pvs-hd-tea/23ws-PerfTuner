for (int i = 0; i < n; i+=2)
{
    c[i] = a[i]*b[i] - a[i+1]*b[i+1];    // calculate the real part
    c[i+1] = a[i]*b[i+1] + a[i+1]*b[i];  // calculate the imaginary part
}