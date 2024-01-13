#include <iostream>

int function(int a, int b);

int main() {
    int c;
    c = function(1,2);
    if (c == 3) std::cout << "The result is the same." << std::endl;
    return 0;
}
