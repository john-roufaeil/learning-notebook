#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Point.h"
#include "Shape.h"
#include <iostream>

class Rectangle : public Shape {
    private:
        Point upperLeft;
        Point lowerRight;
    public:
        Rectangle() {
            upperLeft = Point();
            lowerRight = Point();
        }

        Rectangle(int x1, int y1, int x2, int y2) {
            upperLeft = Point(x1, y1);
            lowerRight = Point(x2, y2);
        }

        void draw() {
            std::cout << "Drawing rectangle with upper-left corner at (" << upperLeft.getX() << ", " << upperLeft.getY() << ") and lower-right corner at ("
                      << lowerRight.getX() << ", " << lowerRight.getY() << ")\n";
        }
};
#endif