#include <iostream>
#include <complex>
#include <vector>
#include <cmath>

const double PI = 3.141592653589793238460;

using Complex = std::complex<double>;

// Perform in-place FFT
void fft(std::vector<Complex>& a, bool invert) {
    int n = a.size();

    // Bit-reverse permutation
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        while (j & bit) {
            j ^= bit;
            bit >>= 1;
        }
        j ^= bit;

        if (i < j) {
            std::swap(a[i], a[j]);
        }
    }

    // Cooley-Tukey iterative FFT
    for (int len = 2; len <= n; len <<= 1) {
        double angle = 2 * PI / len * (invert ? -1 : 1);
        Complex wlen(std::cos(angle), std::sin(angle));
        for (int i = 0; i < n; i += len) {
            Complex w(1);
            for (int j = 0; j < len / 2; ++j) {
                Complex u = a[i + j];
                Complex v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }

    if (invert) {
        for (int i = 0; i < n; ++i) {
            a[i] /= n;
        }
    }
}

int main() {
    // Example usage
    std::vector<Complex> a = {1, 2, 3, 4, 5, 6, 7, 8};

    // Forward FFT
    fft(a, false);

    // Print the FFT result
    std::cout << "FFT Result: ";
    for (const auto& val : a) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Inverse FFT
    fft(a, true);

    // Print the inverse FFT result
    std::cout << "Inverse FFT Result: ";
    for (const auto& val : a) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    return 0;
}
