#include <iostream>
#include "complex.h"

int main() {
    Complex c1;
    std::cin >> c1;
    Complex c2(5, -2);
    Complex c3;
    std::cout << "__________" << std::endl << std::endl;

    std::cout << "c1: "; std::cout << c1 << std::endl;
    std::cout << "c2: "; std::cout << c2; std::cout << std::endl;
    std::cout << "c3: "; std::cout << c3; std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "c1[0]: " << c1[0] << std::endl;
    std::cout << "c1[1]: " << c1[1] << std::endl;
    std::cout << std::endl;

    Complex add = c1 + c2;
    Complex sub = c1 - c2;
    std::cout << c1; std::cout << " + "; std::cout << c2; std::cout << " : "; std::cout << add; std::cout << std::endl;
    std::cout << c1; std::cout << " - "; std::cout << c2; std::cout << " : "; std::cout << sub; std::cout << std::endl;
    std::cout << std::endl;

    Complex c4(2, 3); 
    Complex addNum = c4 + 5.0;
    Complex subNum = c4 - 2.0;
    std::cout << c4; std::cout << " + "; std::cout << 5.0; std::cout << " : "; std::cout << addNum; std::cout << std::endl;
    std::cout << c4; std::cout << " - "; std::cout << 2.0; std::cout << " : "; std::cout << subNum; std::cout << std::endl;
    std::cout << std::endl;

    Complex c5(2, 3);
    Complex numAdd = 10.0 + c5;
    Complex numSub = 10.0 - c5;
    std::cout << "10 + "; std::cout << c5; std::cout << " : "; std::cout << numAdd; std::cout << std::endl;
    std::cout << "10 - "; std::cout << c5; std::cout << " : "; std::cout << numSub; std::cout << std::endl;
    std::cout << std::endl;

    Complex c6(5, 2);
    std::cout << "Initial: "; std::cout << c6; std::cout << std::endl;
    c6 += Complex(6, 3);
    std::cout << "c6 += (6+3i): "; std::cout << c6; std::cout << std::endl;
    std::cout << std::endl;

    Complex c7(4, 3);
    std::cout << "Initial c7: "; std::cout << c7; std::cout << std::endl;
    c7 -= 2.0;
    std::cout << "After c7 -= 2: "; std::cout << c7; std::cout << std::endl;
    std::cout << std::endl;

    Complex c8(3, 4);
    Complex c9(3, 4);
    Complex c10(5, 2);
    std::cout << c8; std::cout << "=="; std::cout << c9; std::cout << (c8 == c9 ? " true" : " false") << std::endl;
    std::cout << c8; std::cout << "=="; std::cout << c10; std::cout << (c8 == c10 ? " true" : " false") << std::endl;
    std::cout << c8; std::cout << "!="; std::cout << c9; std::cout << (c8 != c9 ? " true" : " false") << std::endl;
    std::cout << c8; std::cout << "!="; std::cout << c10; std::cout << (c8 != c10 ? " true" : " false") << std::endl;
    std::cout << c8; std::cout << "<"; std::cout << c10; std::cout << (c8 < c10 ? " true" : " false") << std::endl;
    std::cout << c8; std::cout << ">"; std::cout << c10; std::cout << (c8 > c10 ? " true" : " false") << std::endl;
    std::cout << std::endl;

    Complex c11(5, 3);
    Complex zero;
    // Complex result = c11 / zero;
    Complex cnew = c1;
    std::cout << cnew << std::endl;
    return 0;
}