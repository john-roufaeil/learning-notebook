#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>

enum Key {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    ENTER,
    ESCAPE,
    BACKSPACE,
    HOME,
    END,
    UNKNOWN
};

void gotoxy(int x, int y);
int getColorCode(const std::string &color);
void changeColor(const std::string &textColor);
void resetColor();
void clearScreen();
Key getKeyPress();;
void delay(int milliseconds);
void typeText(const std::string &message, int delayTime);

#endif
