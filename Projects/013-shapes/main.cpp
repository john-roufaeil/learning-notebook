#include <iostream>
#include "shapes.h"

int main() {
    Triangle triangle;
    triangle.setD1(10);
    triangle.setD2(5);
    std::cout << "Triangle area: " << triangle.calculateArea() << std::endl;

    Rectangle rectangle;
    rectangle.setD1(5);
    rectangle.setD2(4);
    std::cout << "Rectangle area: " << rectangle.calculateArea() << std::endl;

    Square square;
    square.setSide(6);
    std::cout << "Square area: " << square.calculateArea() << std::endl;

    Circle circle;
    circle.setRadius(5);
    std::cout << "Circle area: " << circle.calculateArea() << std::endl;

    return 0;
}