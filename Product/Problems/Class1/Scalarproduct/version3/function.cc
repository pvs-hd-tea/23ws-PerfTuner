#include <immintrin.h>
void function(int size, double *vectorA, double *vectorB, double &result) {
    result = 0.0;
    double *endA = vectorA + size;

    while (vectorA < endA) {
        result += (*vectorA) * (*vectorB);
        ++vectorA;
        ++vectorB;
    }
}
