#ifndef COMPLEX_H
#define COMPLEX_H

class Complex {
    private:
        int real;
        int img;
    public:
        Complex(int real = 0, int img = 0);

        void setReal(int real);
        int getReal() const;
        void setImg(int img);
        int getImg() const;
        void displayComplex();

        Complex operator+(const Complex &other) const;
        Complex operator-(const Complex &other) const;
        Complex operator*(const Complex &other) const;
        Complex operator/(const Complex &other) const;
        Complex operator+(int n) const;
        Complex operator-(int n) const;
        Complex operator*(int n) const;
        Complex operator/(int n) const;
        void operator=(const Complex &other);
        void operator+=(const Complex &other);
        void operator-=(const Complex &other);
        void operator*=(const Complex &other);
        void operator/=(const Complex &other);
        void operator+=(int n);
        void operator-=(int n);
        void operator*=(int n);
        void operator/=(int n);
        bool operator==(const Complex &other) const;
        bool operator!=(const Complex &other) const;
        int magnitude() const;
        Complex conjugate() const;

        // reverse
};

#endif