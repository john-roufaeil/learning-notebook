#include <iostream>
#include "complex.h"

void Complex::setReal(int real)  {
    this->real = real;
}

int Complex::getReal() {
    return this->real;
}

void Complex::setImg(int img)  {
    this->img = img;
}

int Complex::getImg() {
    return this->img;
}

void Complex::displayComplex() {
    std::cout << this->getReal() << "+" << this->getImg() << "i" << std::endl;
    std::cout << "==========" << std::endl;
}

Complex::Complex(int real, int img) {
    setReal(real);
    setImg(img);
}