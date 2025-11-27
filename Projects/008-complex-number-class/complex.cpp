#include <iostream>
#include "complex.h"
#include <cmath>

void Complex::setReal(int real)  {
    this->real = real;
}

int Complex::getReal() const {
    return this->real;
}

void Complex::setImg(int img)  {
    this->img = img;
}

int Complex::getImg() const {
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

Complex::Complex(int real, int img) {
    setReal(real);
    setImg(img);
}

Complex Complex::operator+(const Complex &other) const {
    int resultReal = this->getReal() + other.getReal();
    int resultImg = this->getImg() + other.getImg();
    return Complex(resultReal, resultImg);
}

Complex Complex::operator-(const Complex &other) const {
    int resultReal = this->getReal() - other.getReal();
    int resultImg = this->getImg() - other.getImg();
    return Complex(resultReal, resultImg);
}

Complex Complex::operator*(const Complex &other) const {
    int resultReal = this->getReal() * other.getReal() - this->getImg() * other.getImg();
    int resultImg = this->getImg() * other.getReal() + this->getReal() * other.getImg();
    return Complex(resultReal, resultImg);
}

Complex Complex::operator/(const Complex &other) const {
    int thisReal = this->getReal();
    int thisImg = this->getImg();
    int otherReal = other.getReal();
    int otherImg = other.getImg();
    int resultRealNumerator = thisReal * otherReal + thisImg * otherImg;
    int resultImgNumerator = thisImg * otherReal - thisReal * otherImg;
    int resultDenominator = otherReal * otherReal + otherImg * otherImg;
    return Complex(resultRealNumerator / resultDenominator, resultImgNumerator / resultDenominator);
}

Complex Complex::operator+(int n) const {
    return Complex(this->getReal() + n, this->getImg());
}

Complex Complex::operator-(int n) const {
    return Complex(this->getReal() - n, this->getImg());
}

Complex Complex::operator*(int n) const {
    return Complex(this->getReal() * n, this->getImg());
}

Complex Complex::operator/(int n) const {
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
    this->setReal(this->getReal() * other.getReal() - this->getImg() * other.getImg());
    this->setImg(this->getImg() * other.getReal() + this->getReal() * other.getImg());
}

void Complex::operator/=(const Complex &other) {
    int thisReal = this->getReal();
    int thisImg = this->getImg();
    int otherReal = other.getReal();
    int otherImg = other.getImg();
    int resultRealNumerator = thisReal * otherReal + thisImg * otherImg;
    int resultImgNumerator = thisImg * otherReal - thisReal * otherImg;
    int resultDenominator = otherReal * otherReal + otherImg * otherImg;
    this->setReal(resultRealNumerator / resultDenominator);
    this->setImg(resultImgNumerator / resultDenominator);
}

void Complex::operator+=(int n) {
    this->setReal(this->getReal() + n);
}

void Complex::operator-=(int n) {
    this->setReal(this->getReal() - n);
}

void Complex::operator*=(int n) {
    this->setReal(this->getReal() * n);
}

void Complex::operator/=(int n) {
    this->setReal(this->getReal() / n);
}

bool Complex::operator==(const Complex &other) const {
    return this->getReal() == other.getReal() && this->getImg() == other.getImg();
}

bool Complex::operator!=(const Complex &other) const {
    return this->getReal() != other.getReal() || this->getImg() != other.getImg();
}

int Complex::magnitude() const {
    int underRoot = this->getImg() * this->getImg() + this->getReal() * this->getReal();
    return sqrt(underRoot);
}

Complex Complex::conjugate() const {
    return Complex(this->getReal(), -1 * this->getImg());
}