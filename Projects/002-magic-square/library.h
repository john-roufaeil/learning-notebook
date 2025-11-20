#ifndef LIBRARY_H
#define LIBRARY_H

// #include <vector>
// #include <functional>
#include <string>
#include <iostream>

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <unistd.h>
    #include <termios.h>
#endif

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
int getColorCode(std::string color, int isWindows);
void changeColor(std::string textColor);
void resetColor();
void clearScreen();
Key getKeyPress();;
void delay(int milliseconds);
void typeText(std::string message, int delayTime);

#endif
