#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>
#include <cstdlib>
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
int getColorCode(std::string color);
void changeColor(std::string textColor);
void resetColor();
void clearScreen();
Key getKeyPress();

#endif
