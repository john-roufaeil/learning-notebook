#include "shapes.h"

float Shape::getD1() {
    return d1;
}
float Shape::getD2() {
    return d2;
}
void Shape::setD1(float d1) {
    this->d1 = d1;
}
void Shape::setD2(float d2) {
    this->d2 = d2;
}


float Triangle::calculateArea() {
    return 0.5f * d1 * d2;
}


float Rectangle::calculateArea() {
    return d1 * d2;
}


void Square::setSide(float d) {
    setD1(d);
    setD2(d);
}
float Square::getSide() {
    return getD1();
}
float Square::calculateArea() {
    return d1 * d1;
}


void Circle::setRadius(float r) {
    setD1(r);
    setD2(r);
}
float Circle::getRadius() {
    return getD1();
}
float Circle::calculateArea() {
    return 3.14f * d1 * d1;
}
