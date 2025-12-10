#include "swap.h"

int main() {
    int a = 1;
    int b = 2;

    std::cout << "Before swap: a = " << a << ", b = " << b << std::endl;

    swap_temp(&a, &b);
    std::cout << "After swap_temp: a = " << a << ", b = " << b << std::endl;
    
    swap_addition(&a, &b);
    std::cout << "After swap_addition: a = " << a << ", b = " << b << std::endl;

    swap_multiplication(&a, &b);
    std::cout << "After swap_multiplication: a = " << a << ", b = " << b << std::endl;

    swap_xor(&a, &b);
    std::cout << "After swap_xor: a = " << a << ", b = " << b << std::endl;

    return 0;
}