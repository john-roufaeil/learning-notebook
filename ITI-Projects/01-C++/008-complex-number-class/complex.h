#ifndef COMPLEX_H
#define COMPLEX_H

#include <iostream>

class Complex {
    private:
        double real;
        double img;
    public:
        Complex(double real = 0, double img = 0);
        Complex(const Complex &other);
        Complex& operator=(const Complex& other);
        /* Useless to have move constructor & assignment
           data is only doubles will be copied anyway(no heap resources to steal)
           Complex(Complex &&other); 
           Complex &operator=(Complex &&other); */

        void setReal(double real);
        double getReal() const;
        void setImg(double img);
        double getImg() const;
        
        Complex operator+() const;
        Complex operator-() const;

        Complex operator+(const Complex &other) const;
        Complex operator-(const Complex &other) const;
        Complex operator*(const Complex &other) const;
        Complex operator/(const Complex &other) const;
        Complex operator+(double n) const;
        Complex operator-(double n) const;
        Complex operator*(double n) const;
        Complex operator/(double n) const;

        double operator[](int n) const;

        Complex& operator++(); // Prefix
        Complex& operator--();
        Complex operator++(int); // Postfix
        Complex operator--(int);
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
        bool operator<(const Complex& other) const;
        bool operator>(const Complex& other) const;
        bool operator<=(const Complex& other) const;
        bool operator>=(const Complex& other) const;

        double operator()() const;
        Complex conjugate() const;
};

Complex operator+(double n, const Complex& c);
Complex operator-(double n, const Complex& c);
Complex operator*(double n, const Complex& c);
Complex operator/(double n, const Complex& c);
std::ostream& operator<<(std::ostream& os, const Complex& c);
std::istream& operator>>(std::istream& is, Complex& c);

#endif