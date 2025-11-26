#ifndef COMPLEX_H
#define COMPLEX_H

class Complex {
    private:
        int real;
        int img;
    public:
        void setReal(int real);
        int getReal();
        void setImg(int img);
        int getImg();
        void displayComplex();
        Complex(int real = 0, int img = 0);
};

#endif