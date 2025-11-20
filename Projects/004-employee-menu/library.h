#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>
#include <iostream>

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <unistd.h>
    #include <termios.h>
#endif

struct DOB {
    int day;
    int month;
    int year;
};

struct Employee {
    std::string firstName;
    std::string lastName;
    int age;
    int salary;
    DOB dob;
};

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
    TAB,
    UNKNOWN
};

void gotoxy(int x, int y);
int getColorCode(std::string color, int isWindows);
void changeColor(std::string textColor);
void resetColor();
void clearScreen();
Key getKeyPress();

#endif
