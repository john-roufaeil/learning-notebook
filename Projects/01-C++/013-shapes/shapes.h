#ifndef SHAPES_H
#define SHAPES_H

class Shape {
    protected:
        float d1;
        float d2;
    
    public:
        virtual float calculateArea() = 0;
        float getD1();
        float getD2();
        void setD1(float d1);
        void setD2(float d2);
};

class Triangle : public Shape {
    public:
        float calculateArea() override;
};

class Rectangle : public Shape {
    public:
        float calculateArea() override;
};

class Square : private Rectangle {
    public:
        void setSide(float d);
        float getSide();
        float calculateArea();
};

class Circle : private Shape {
    public:
        void setRadius(float r);
        float getRadius();
        float calculateArea();
};

#endif