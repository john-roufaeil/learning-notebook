#ifndef SHAPES_H
#define SHAPES_H

class Shape {
    protected:
        float d1;
        float d2;
    
    public:
        virtual float calculateArea() = 0;
        float getD1() {
            return d1;
        }
        float getD2() {
            return d2;
        }
        void setD1(float d1) {
            this->d1 = d1;
        }
        void setD2(float d2) {
            this->d2 = d2;
        }
};

class Triangle : public Shape {
    public:
        float calculateArea() override {
            return 0.5f * d1 * d2;
        }
};

class Rectangle : public Shape {
    public: 
        float calculateArea() override {
            return d1 * d2;
        }
};

class Square : private Rectangle {
    public:
        void setSide(float d) {
            setD1(d);
            setD2(d);
        }

        float getSide() {
            return getD1();
        }

        float calculateArea() {
            return d1 * d1;
        }
};

class Circle : private Shape {
    public:
        void setRadius(float r) {
            setD1(r);
            setD2(r);
        }

        float getRadius() {
            return getD1();
        }

        float calculateArea() {
            return 3.14f * d1 * d1;
        }
};

#endif