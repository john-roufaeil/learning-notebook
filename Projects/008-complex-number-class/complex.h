#ifndef COMPLEX_H
#define COMPLEX_H

#include <iostream>

class Complex {
    private:
        double real;
        double img;
    public:
        Complex(double real = 0, double img = 0);

        void setReal(double real);
        double getReal() const;
        void setImg(double img);
        double getImg() const;

        Complex operator+(const Complex &other) const;
        Complex operator-(const Complex &other) const;
        Complex operator*(const Complex &other) const;
        Complex operator/(const Complex &other) const;
        Complex operator+(double n) const;
        Complex operator-(double n) const;
        Complex operator*(double n) const;
        Complex operator/(double n) const;
        Complex operator[](int n) const;
        Complex operator++(int); // Postfix
        Complex operator--(int);
        Complex& operator++(); // Prefix
        Complex& operator--();
        Complex& operator=(const Complex& other);
        Complex& operator+=(const Complex& other);
        Complex& operator-=(const Complex& other);
        Complex& operator*=(const Complex& other);
        Complex& operator/=(const Complex& other);
        Complex& operator+=(double n);
        Complex& operator-=(double n);
        Complex& operator*=(double n);
        Complex& operator/=(double n);
        bool operator==(const Complex &other) const;
        bool operator!=(const Complex &other) const;
        double magnitude() const;
        Complex conjugate() const;
    };
    
Complex operator+(double n, const Complex& c);
Complex operator-(double n, const Complex& c);
Complex operator*(double n, const Complex& c);
Complex operator/(double n, const Complex& c);
std::ostream& operator<<(std::ostream& os, const Complex& c);
std::istream& operator>>(std::istream& is, Complex& c);

#endif