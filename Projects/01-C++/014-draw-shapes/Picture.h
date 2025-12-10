#ifndef PICTURE_H
#define PICTURE_H

#include "Point.h"
#include "Line.h"
#include "Rectangle.h"
#include "Circle.h"
#include "Shape.h"
#include <iostream>

class Picture {
    private:
        Shape **shapes;
        int count;
        int capacity;
    public:
        Picture(int initialCapacity = 10) {
            count = 0;
            capacity = initialCapacity;
            shapes = new Shape *[capacity];
        }

        ~Picture() {
            delete[] shapes;
        }

        void addShape(Shape *shape) {
            if (count == capacity) {
                capacity *= 2;
                Shape **newShapes = new Shape *[capacity];
                for (int i = 0; i < count; i++) {
                    newShapes[i] = shapes[i];
                }
                delete[] shapes;
                shapes = newShapes;
            }
            shapes[count] = shape;
            count++;
        }

        void draw() {
            for (int i = 0; i < count; i++) {
                shapes[i]->draw();
            }
        }
};
#endif