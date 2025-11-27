#include <iostream>
#include "complex.h"

int main() {
    // Constructor and Display
    Complex c1(3, 4);
    Complex c2(5, -2);
    Complex c3;
    std::cout << "c1(3, 4): ";  c1.display(); std::cout << std::endl;
    std::cout << "c2(5, -2): "; c2.display(); std::cout << std::endl;
    std::cout << "c3: "; c3.display(); std::cout << std::endl;
    std::cout << std::endl;

    // Arithmetic (Complex op Complex)
    Complex add = c1 + c2;
    Complex sub = c1 - c2;
    c1.display(); std::cout << " + "; c2.display(); std::cout << " : "; add.display(); std::cout << std::endl;
    c1.display(); std::cout << " - "; c2.display(); std::cout << " : "; sub.display(); std::cout << std::endl;
    std::cout << std::endl;

    // Artihmetic (Complex op double)
    Complex c4(2, 3); 
    Complex addNum = c4 + 5.0;
    Complex subNum = c4 - 2.0;
    c4.display(); std::cout << " + "; std::cout << 5.0; std::cout << " : "; addNum.display(); std::cout << std::endl;
    c4.display(); std::cout << " - "; std::cout << 2.0; std::cout << " : "; subNum.display(); std::cout << std::endl;
    std::cout << std::endl;

    // Arithmetic (double op Complex)
    Complex c5(2, 3);
    Complex numAdd = 10.0 + c5;
    Complex numSub = 10.0 - c5;
    std::cout << "10 + "; c5.display(); std::cout << " : "; numAdd.display(); std::cout << std::endl;
    std::cout << "10 - "; c5.display(); std::cout << " : "; numSub.display(); std::cout << std::endl;
    std::cout << std::endl;

    // Compound Assignment
    Complex c6(5, 2);
    std::cout << "Initial: "; c6.display(); std::cout << std::endl;
    c6 += Complex(6, 3);
    std::cout << "c6 += (6+3i): "; c6.display(); std::cout << std::endl;
    std::cout << std::endl;

    //Num Compound Assignment
    Complex c7(4, 3);
    std::cout << "Initial c7: "; c7.display(); std::cout << std::endl;
    c7 -= 2.0;
    std::cout << "After c7 -= 2: "; c7.display(); std::cout << std::endl;
    std::cout << std::endl;

    // Equality Check
    Complex c8(3, 4);
    Complex c9(3, 4);
    Complex c10(5, 2);
    c8.display(); std::cout << "=="; c9.display(); std::cout << (c8 == c9 ? " true" : " false") << std::endl;
    c8.display(); std::cout << "=="; c10.display(); std::cout << (c8 == c10 ? " true" : " false") << std::endl;
    c8.display(); std::cout << "!="; c9.display(); std::cout << (c8 != c9 ? " true" : " false") << std::endl;
    c8.display(); std::cout << "!="; c10.display(); std::cout << (c8 != c10 ? " true" : " false") << std::endl;
    std::cout << std::endl;

    Complex c11(5, 3);
    Complex zero(0, 0);
    // Complex result = c11 / zero;

    return 0;
}