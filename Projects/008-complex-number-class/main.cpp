#include <iostream>
#include "complex.h"

int main() {
    Complex c = Complex(10, 20);
    Complex c2 = Complex();

    c.displayComplex();
    c2.displayComplex();
    return 0;
}