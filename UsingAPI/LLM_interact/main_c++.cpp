Here is the optimized function along with the main function, header files, and input data:

```cpp
#include <iostream>

int addTwoNumbers(int a, int b) {
    return a + b;
}

int main() {
    int num1, num2;
    
    std::cout << "Enter number 1: ";
    std::cin >> num1;
    
    std::cout << "Enter number 2: ";
    std::cin >> num2;
    
    int result = addTwoNumbers(num1, num2);
    
    std::cout << "The sum of the two numbers is: " << result << std::endl;
    
    return 0;
}
```