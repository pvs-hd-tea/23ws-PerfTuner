#include <immintrin.h>
void function(int n,double* x, double* y, double* z)
{
    for (int i = 0; i < n; i+=2)
    {
        z[i] = x[i]*y[i] - x[i+1]*y[i+1];    // calculate the real part
        z[i+1] = x[i]*y[i+1] + x[i+1]*y[i];  // calculate the imaginary part
    }
}