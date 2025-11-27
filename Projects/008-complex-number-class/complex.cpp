#include <iostream>
#include "complex.h"
#include <cmath>
#include <stdexcept>

#define EPSILON 1e-9

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
    if (resultDenominator < EPSILON)
        throw std::invalid_argument("Division by zero");
    return Complex(resultRealNumerator / resultDenominator, resultImgNumerator / resultDenominator);
}

Complex Complex::operator+(double n) const {
    return Complex(this->getReal() + n, this->getImg());
}

Complex Complex::operator-(double n) const {
    return Complex(this->getReal() - n, this->getImg());
}

Complex Complex::operator*(double n) const {
    return Complex(this->getReal() * n, this->getImg() * n);
}

Complex Complex::operator/(double n) const {   
    if (abs(n) < EPSILON)
        throw std::invalid_argument("Division by zero");
    return Complex(this->getReal() / n, this->getImg() / n);
}

Complex Complex::operator++(int) {
    Complex before = *this;
    this->setReal(this->getReal() + 1);
    return before;
}

Complex Complex::operator--(int) {
    Complex before = *this;
    this->setReal(this->getReal() - 1);
    return before;
}

Complex& Complex::operator++() {
    this->setReal(this->getReal() + 1);
    return *this;
}

Complex& Complex::operator--() {
    this->setReal(this->getReal() - 1);
    return *this;
}

Complex& Complex::operator=(const Complex &other) {
    this->setReal(other.getReal());
    this->setImg(other.getImg());
    return *this;
}

Complex& Complex::operator+=(const Complex &other) {
    this->setReal(other.getReal() + this->getReal());
    this->setImg(other.getImg() + this->getImg());
    return *this;
}

Complex& Complex::operator-=(const Complex &other) {
    this->setReal(this->getReal() - other.getReal());
    this->setImg(this->getImg() - other.getImg());
    return *this;
}

Complex& Complex::operator*=(const Complex &other) {
    double thisReal = this->getReal();
    double thisImg = this->getImg();
    double otherReal = other.getReal();
    double otherImg = other.getImg();
    this->setReal(thisReal * otherReal - thisImg * otherImg);
    this->setImg(thisImg * otherReal + thisReal * otherImg);
    return *this;
}

Complex& Complex::operator/=(const Complex &other) {
    double thisReal = this->getReal();
    double thisImg = this->getImg();
    double otherReal = other.getReal();
    double otherImg = other.getImg();
    double resultRealNumerator = thisReal * otherReal + thisImg * otherImg;
    double resultImgNumerator = thisImg * otherReal - thisReal * otherImg;
    double resultDenominator = otherReal * otherReal + otherImg * otherImg;
    if (resultDenominator < EPSILON)
        throw std::invalid_argument("Division by zero");
    this->setReal(resultRealNumerator / resultDenominator);
    this->setImg(resultImgNumerator / resultDenominator);
    return *this;
}

Complex& Complex::operator+=(double n) {
    this->setReal(this->getReal() + n);
    return *this;
}

Complex& Complex::operator-=(double n) {
    this->setReal(this->getReal() - n);
    return *this;
}

Complex& Complex::operator*=(double n) {
    this->setReal(this->getReal() * n);
    this->setImg(this->getImg() * n);
    return *this;
}

Complex& Complex::operator/=(double n) {
    if (abs(n) < EPSILON)
        throw std::invalid_argument("Division by zero");
    this->setReal(this->getReal() / n);
    this->setImg(this->getImg() / n);
    return *this;
}

bool Complex::operator==(const Complex &other) const {
    return abs(this->getReal() - other.getReal()) < EPSILON && abs(this->getImg() - other.getImg()) < EPSILON;
}

bool Complex::operator!=(const Complex &other) const {
    return abs(this->getReal() - other.getReal()) > EPSILON || abs(this->getImg() - other.getImg()) > EPSILON;
}

double Complex::magnitude() const {
    double underRoot = this->getImg() * this->getImg() + this->getReal() * this->getReal();
    return sqrt(underRoot);
}

Complex Complex::conjugate() const {
    return Complex(this->getReal(), -1 * this->getImg());
}

Complex operator+(double n, const Complex& c) {
    return Complex(n + c.getReal(), c.getImg());
}

Complex operator-(double n, const Complex& c) {
    return Complex(n - c.getReal(), -1 * c.getImg());
}

Complex operator*(double n, const Complex& c) {
    return Complex(n * c.getReal(), n * c.getImg());
}

Complex operator/(double n, const Complex& c) {
    // n / (a+bi) = n * (a-bi) / (a²+b²)
    double denominator = c.getReal() * c.getReal() + c.getImg() * c.getImg();
    if (abs(denominator) < EPSILON)
        throw std::invalid_argument("Division by zero");
    return Complex(n * c.getReal() / denominator, -n * c.getImg() / denominator);
}