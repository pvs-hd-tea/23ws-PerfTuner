#include <immintrin.h>

void function(int size, double scalarA, double *vectorB, double scalarC, double *vectorD, double *resultVector)
{
    for (int index = 0; index < size; index++)
    {
        resultVector[index] = scalarA * vectorB[index] + scalarC * vectorD[index];
    }
}
