#ifndef LINE_H
#define LINE_H

#include "Point.h"
#include "Shape.h"
#include <iostream>

class Line : public Shape {
    private:
        Point start;
        Point end;
    public:
        Line() {
            start = Point();
            end = Point();
        }

        Line(int x1, int y1, int x2, int y2) {
            start = Point(x1, y1);
            end = Point(x2, y2);
        }

        void draw() {
            std::cout << "Drawing line from (" << start.getX() << ", " << start.getY() << ") to ("
                      << end.getX() << ", " << end.getY() << ")\n";
        }
};

#endif