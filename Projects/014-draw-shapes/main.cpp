#include "Picture.h"

int main () {
    Picture p;
    p.addShape(new Line());
    p.addShape(new Line(10,10,100,100));
    p.addShape(new Line(20,20,200,200));
    p.addShape(new Rectangle());
    p.addShape(new Rectangle(15, 15, 60, 60));
    p.addShape(new Rectangle(25, 25, 80, 80));
    p.addShape(new Circle());
    p.addShape(new Circle(50, 50, 25));
    p.addShape(new Circle(75, 75, 35));
    p.draw();
    return 0;
}