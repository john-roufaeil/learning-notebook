#ifndef COMPLEX_H
#define COMPLEX_H

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
        void displayComplex();

        Complex operator+(const Complex &other) const;
        Complex operator-(const Complex &other) const;
        Complex operator*(const Complex &other) const;
        Complex operator/(const Complex &other) const;
        Complex operator+(double n) const;
        Complex operator-(double n) const;
        Complex operator*(double n) const;
        Complex operator/(double n) const;
        Complex& operator=(const Complex &other);
        void operator+=(const Complex &other);
        void operator-=(const Complex &other);
        void operator*=(const Complex &other);
        void operator/=(const Complex &other);
        void operator+=(double n);
        void operator-=(double n);
        void operator*=(double n);
        void operator/=(double n);
        bool operator==(const Complex &other) const;
        bool operator!=(const Complex &other) const;
        double magnitude() const;
        Complex conjugate() const;

        // reverse
};

#endif