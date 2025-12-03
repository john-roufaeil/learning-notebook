#ifndef PICTURE_H
#define PICTURE_H

#include "Point.h"
#include "Line.h"
#include "Rectangle.h"
#include "Circle.h"
#include <iostream>

class Picture {
    private:
        int linesCount;
        int rectanglesCount;
        int circlesCount;
        Line *pLines;
        Rectangle *pRectangles;
        Circle *pCircles;
    public:
        Picture() {
            linesCount = 0;
            rectanglesCount = 0;
            circlesCount = 0;
            pLines = nullptr;
            pRectangles = nullptr;
            pCircles = nullptr;
        }

        Picture(int lc, int rc, int cc, Line* lines, Rectangle* rectangles, Circle* circles) {
            linesCount = lc;
            rectanglesCount = rc;
            circlesCount = cc;
            pLines = lines;
            pRectangles = rectangles;
            pCircles = circles;
        }

        void setLines(int lc, Line* lines) {
            linesCount = lc;
            pLines = lines;
        }

        void setRectangles(int rc, Rectangle* rectangles) {
            rectanglesCount = rc;
            pRectangles = rectangles;
        }

        void setCircles(int cc, Circle* circles) {
            circlesCount = cc;
            pCircles = circles;
        }

        void draw() {
            for (int i = 0; i < linesCount; i++) {
                pLines[i].draw();
            }
            for (int i = 0; i < rectanglesCount; i++) {
                pRectangles[i].draw();
            }
            for (int i = 0; i < circlesCount; i++) {
                pCircles[i].draw();
            }
        }
};
#endif