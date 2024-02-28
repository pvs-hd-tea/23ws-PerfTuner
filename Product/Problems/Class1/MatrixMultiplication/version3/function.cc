#include <immintrin.h>
void function(int numRowsA, int numColsA, int numColsB, double *matrixA, double *matrixB, double *resultMatrix) {
    for (int row = 0; row < numRowsA; ++row) {
        for (int col = 0; col < numColsB; ++col) {
            double sum = 0.0;
            for (int i = 0; i < numColsA; ++i) {
                sum += *(matrixA + row * numColsA + i) * *(matrixB + i * numColsB + col);
            }
            *(resultMatrix + row * numRowsA + col) = sum;
        }
    }
}