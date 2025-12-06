#include <iostream>
#include "complex.h"
#include <cmath>
#include <stdexcept>
#include <string>

#define EPSILON 1e-9

Complex::Complex(double real, double img) {
    this->real = real;
    this->img = img;
}

Complex::Complex(const Complex &other) {
    real = other.real;
    img = other.img;
}

Complex& Complex::operator=(const Complex &other) {
    if (this != &other) {
        real = other.real;
        img = other.img;
    }
    return *this;
}

void Complex::setReal(double real)  {
    this->real = real;
}

double Complex::getReal() const {
    return real;
}

void Complex::setImg(double img)  {
    this->img = img;
}

double Complex::getImg() const {
    return img;
}

Complex Complex::operator+() const {
    return *this;
}

Complex Complex::operator-() const {
    return Complex(-real, -img);
}

Complex Complex::operator+(const Complex &other) const {
    return Complex(real + other.real, img + other.img);
}

Complex Complex::operator-(const Complex &other) const {
    return Complex(real - other.real, img - other.img);
}

Complex Complex::operator*(const Complex &other) const {
    double resultReal = real * other.real - img * other.img;
    double resultImg = img * other.real + real * other.img;
    return Complex(resultReal, resultImg);
}

Complex Complex::operator/(const Complex &other) const {
    double resultRealNumerator = real * other.real + img * other.img;
    double resultImgNumerator = img * other.real - real * other.img;
    double resultDenominator = other.real * other.real + other.img * other.img;
    if (resultDenominator < EPSILON)
        throw std::invalid_argument("Division by zero");
    return Complex(resultRealNumerator / resultDenominator, resultImgNumerator / resultDenominator);
}

double Complex::operator[](int n) const {
    if (n == 0) return real;
    return img;
}

Complex Complex::operator+(double n) const {
    return Complex(real + n, img);
}

Complex Complex::operator-(double n) const {
    return Complex(real - n, img);
}

Complex Complex::operator*(double n) const {
    return Complex(real * n, img * n);
}

Complex Complex::operator/(double n) const {   
    if (abs(n) < EPSILON) throw std::invalid_argument("Division by zero");
    return Complex(real / n, img / n);
}

Complex Complex::operator++(int) {
    Complex before = *this;
    real++;
    return before;
}

Complex Complex::operator--(int) {
    Complex before = *this;
    real--;
    return before;
}

Complex& Complex::operator++() {
    real++;
    return *this;
}

Complex& Complex::operator--() {
    real--;
    return *this;
}

Complex& Complex::operator+=(const Complex &other) {
    real += other.real;
    img += other.img;
    return *this;
}

Complex& Complex::operator-=(const Complex &other) {
    real -= other.real;
    img -= other.img;
    return *this;
}

Complex& Complex::operator*=(const Complex &other) {
    double newReal = real * other.real - img * other.img;
    double newImg = img * other.real + real * other.img;
    real = newReal;
    img = newImg;
    return *this;
}

Complex& Complex::operator/=(const Complex &other) {
    double resultRealNumerator = real * other.real + img * other.img;
    double resultImgNumerator = img * other.real - real * other.img;
    double resultDenominator = other.real * other.real + other.img * other.img;

    if (resultDenominator < EPSILON) throw std::invalid_argument("Division by zero");
    real = resultRealNumerator / resultDenominator;
    img = resultImgNumerator / resultDenominator;
    return *this;
}

Complex& Complex::operator+=(double n) {
    real += n;
    return *this;
}

Complex& Complex::operator-=(double n) {
    real -= n;
    return *this;
}

Complex& Complex::operator*=(double n) {
    real *= n;
    img *= n;
    return *this;
}

Complex& Complex::operator/=(double n) {
    if (abs(n) < EPSILON) throw std::invalid_argument("Division by zero");
    real /= n;
    img /= n;
    return *this;
}

bool Complex::operator==(const Complex &other) const {
    return (abs(real - other.real) < EPSILON) && (abs(img - other.img) < EPSILON);
}

bool Complex::operator!=(const Complex &other) const {
    return (abs(real - other.real) > EPSILON) || (abs(img - other.img) > EPSILON);
}

bool Complex::operator<(const Complex& other) const {
    return (*this)() < other();
}

bool Complex::operator>(const Complex& other) const {
    return (*this)() > other();
}

bool Complex::operator<=(const Complex& other) const {
    return (*this)() <= other();
}

bool Complex::operator>=(const Complex& other) const{
    return (*this)() >= other();
}

double Complex::operator()() const {
    double underRoot = (img * img) + (real * real);
    return sqrt(underRoot);
}

Complex Complex::conjugate() const {
    return Complex(real, -img);
}

//--- non-member functions ---//

Complex operator+(double n, const Complex& c) {
    return Complex(n + c.getReal(), c.getImg());
}

Complex operator-(double n, const Complex& c) {
    return Complex(n - c.getReal(), -c.getImg());
}

Complex operator*(double n, const Complex& c) {
    return Complex(n * c.getReal(), n * c.getImg());
}

Complex operator/(double n, const Complex& c) {
    // n / (a+bi) = n * (a-bi) / (a²+b²)
    double denominator = c.getReal() * c.getReal() + c.getImg() * c.getImg();
    if (abs(denominator) < EPSILON) throw std::invalid_argument("Division by zero");
    return Complex((n * c.getReal()) / denominator, (-n * c.getImg()) / denominator);
}

std::ostream& operator<<(std::ostream& os, const Complex& c) { 
    double real = c.getReal();
    double img = c.getImg();
    if (abs(real) < EPSILON && abs(img) < EPSILON) return os << "0";
    else if (abs(real) < EPSILON) {
        if (abs(img - 1) < EPSILON) return os << "i";
        if (abs(img + 1) < EPSILON) return os << "-i";
        return os << img << "i";
    } else {
        os << real;
        if (abs(img - 1) < EPSILON) os << "+i";
        else if (abs(img + 1) < EPSILON) os << "-i";
        else if (img > 0) os << "+" << img << "i";
        else os << img << "i";
    }
    return os;
}

std::istream& operator>>(std::istream& is, Complex& c) {
    double real, img;
    std::cout << "Input real: ";
    is >> real;
    std::cout << "Input img: ";
    is >> img;
    c.setReal(real);
    c.setImg(img);
    return is;
}