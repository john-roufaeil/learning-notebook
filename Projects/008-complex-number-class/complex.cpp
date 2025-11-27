#include <iostream>
#include "complex.h"
#include <cmath>

void Complex::setReal(double real)  {
    this->real = real;
}

double Complex::getReal() const {
    return this->real;
}

void Complex::setImg(double img)  {
    this->img = img;
}

double Complex::getImg() const {
    return this->img;
}

void Complex::displayComplex() {
    if (this->getImg() < 0) {
        std::cout << this->getReal() << this->getImg() << "i" << std::endl;
    } else {
        std::cout << this->getReal() << "+" << this->getImg() << "i" << std::endl;
    }
    std::cout << "==========" << std::endl;
}

Complex::Complex(double real, double img) {
    setReal(real);
    setImg(img);
}

Complex Complex::operator+(const Complex &other) const {
    double resultReal = this->getReal() + other.getReal();
    double resultImg = this->getImg() + other.getImg();
    return Complex(resultReal, resultImg);
}

Complex Complex::operator-(const Complex &other) const {
    double resultReal = this->getReal() - other.getReal();
    double resultImg = this->getImg() - other.getImg();
    return Complex(resultReal, resultImg);
}

Complex Complex::operator*(const Complex &other) const {
    double resultReal = this->getReal() * other.getReal() - this->getImg() * other.getImg();
    double resultImg = this->getImg() * other.getReal() + this->getReal() * other.getImg();
    return Complex(resultReal, resultImg);
}

Complex Complex::operator/(const Complex &other) const {
    double thisReal = this->getReal();
    double thisImg = this->getImg();
    double otherReal = other.getReal();
    double otherImg = other.getImg();
    double resultRealNumerator = thisReal * otherReal + thisImg * otherImg;
    double resultImgNumerator = thisImg * otherReal - thisReal * otherImg;
    double resultDenominator = otherReal * otherReal + otherImg * otherImg;
    return Complex(resultRealNumerator / resultDenominator, resultImgNumerator / resultDenominator);
}

Complex Complex::operator+(double n) const {
    return Complex(this->getReal() + n, this->getImg());
}

Complex Complex::operator-(double n) const {
    return Complex(this->getReal() - n, this->getImg());
}

Complex Complex::operator*(double n) const {
    return Complex(this->getReal() * n, this->getImg());
}

Complex Complex::operator/(double n) const {
    return Complex(this->getReal() / n, this->getImg());
}

void Complex::operator=(const Complex &other) {
    this->setReal(other.getReal());
    this->setImg(other.getImg());
}

void Complex::operator+=(const Complex &other) {
    this->setReal(other.getReal() + this->getReal());
    this->setImg(other.getImg() + this->getImg());
}

void Complex::operator-=(const Complex &other) {
    this->setReal(this->getReal() - other.getReal());
    this->setImg(this->getImg() - other.getImg());
}

void Complex::operator*=(const Complex &other) {
    double thisReal = this->getReal();
    double thisImg = this->getImg();
    double otherReal = other.getReal();
    double otherImg = other.getImg();
    this->setReal(thisReal * otherReal - thisImg * otherImg);
    this->setImg(thisImg * otherReal + thisReal * otherImg);
}

void Complex::operator/=(const Complex &other) {
    double thisReal = this->getReal();
    double thisImg = this->getImg();
    double otherReal = other.getReal();
    double otherImg = other.getImg();
    double resultRealNumerator = thisReal * otherReal + thisImg * otherImg;
    double resultImgNumerator = thisImg * otherReal - thisReal * otherImg;
    double resultDenominator = otherReal * otherReal + otherImg * otherImg;
    this->setReal(resultRealNumerator / resultDenominator);
    this->setImg(resultImgNumerator / resultDenominator);
}

void Complex::operator+=(double n) {
    this->setReal(this->getReal() + n);
}

void Complex::operator-=(double n) {
    this->setReal(this->getReal() - n);
}

void Complex::operator*=(double n) {
    this->setReal(this->getReal() * n);
}

void Complex::operator/=(double n) {
    this->setReal(this->getReal() / n);
}

bool Complex::operator==(const Complex &other) const {
    return this->getReal() == other.getReal() && this->getImg() == other.getImg();
}

bool Complex::operator!=(const Complex &other) const {
    return this->getReal() != other.getReal() || this->getImg() != other.getImg();
}

double Complex::magnitude() const {
    double underRoot = this->getImg() * this->getImg() + this->getReal() * this->getReal();
    return sqrt(underRoot);
}

Complex Complex::conjugate() const {
    return Complex(this->getReal(), -1 * this->getImg());
}