#ifndef CIRCLE_H
#define CIRCLE_H

#include "Point.h"
#include "Shape.h"
#include <iostream>

class Circle : public Shape {
    private:
        Point center;
        int radius;
    public:
        Circle() {
            center = Point();
            radius = 0;
        }

        Circle(int x, int y, int radius) {
            center = Point(x, y);
            this->radius = radius;
        }

        void draw() {
            std::cout << "Drawing circle with center at (" << center.getX() << ", " << center.getY() << ") and radius " << radius << "\n";
        }
};
#endif