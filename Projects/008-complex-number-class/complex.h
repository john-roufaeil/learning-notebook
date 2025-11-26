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
};

#endif