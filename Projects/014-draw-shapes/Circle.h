#ifndef CIRCLE_H
#define CIRCLE_H

#include "Point.h"
#include <iostream>

class Circle {
    private:
        Point center;
        int radius;
    public:
        Circle() {
            this->center = Point();
            this->radius = 0;
        }

        Circle(int x, int y, int radius) {
            this->center = Point(x, y);
            this->radius = radius;
        }

        void draw() {
            std::cout << "Drawing circle with center at (" << center.getX() << ", " << center.getY() << ") and radius " << radius << "\n";
        }
};
#endif