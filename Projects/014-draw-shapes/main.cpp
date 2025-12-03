#include "Picture.h"

int main () {
    Line lines[4] = {
        Line(),
        Line(10, 10, 100, 100),
        Line(20, 20, 200, 200),
        Line(30, 30, 300, 300)
    };
    Rectangle rectangles[3] = {
        Rectangle(),
        Rectangle(10, 10, 50, 50),
        Rectangle(20, 20, 100, 100)
    };
    Circle circles[2] = {
        Circle(50, 50, 5),
        Circle(100, 100, 10)
    };
    Picture picture(4, 3, 2, lines, rectangles, circles);
    picture.draw();
    return 0;
}