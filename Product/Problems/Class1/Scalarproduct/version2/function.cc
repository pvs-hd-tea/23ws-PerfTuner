#include <immintrin.h>
void function(int size, double *vectorA, double *vectorB, double &result) {
    result = 0.0;
    for (int index = 0; index < size; ++index) {
        result += vectorA[index] * vectorB[index];
    }
}
