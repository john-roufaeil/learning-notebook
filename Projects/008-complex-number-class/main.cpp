#include <iostream>
#include "complex.h"

int main() {
    Complex c = Complex(5, 9);
    Complex c2 = Complex(-9,1);
    Complex c3 = c + c2;
    Complex c4 = c * c2;
    Complex c5 = c / c2;

    c.displayComplex();
    c2.displayComplex();
    c3.displayComplex();
    c4.displayComplex();
    c5.displayComplex();
    return 0;
}