#ifndef POINT_H
#define POINT_H

class Point {
    private:
        int x;
        int y;
    public:
        Point() {
            this->x = 0;
            this->y = 0;
        }
        Point(int x, int y) {
            this->x = x;
            this->y = y;}

        void setX(int x) {
            this->x = x;
        }
        int getX() const {
            return this->x;
        }

        void setY(int y) {
            this->y = y;
        }
        int getY() const {
            return this->y;
        }
};

#endif