#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>

struct DOB {
    int day;
    int month;
    int year;
};

struct Employee {
    int salary;
    std::string firstName;
    std::string lastName;
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

bool isInt(std::string input);
bool isAlpha(std::string input);
bool isValidDOB(std::string input);
DOB extractDOB(std::string input);
void gotoxy(int x, int y);
int getColorCode(std::string color);
void changeColor(std::string textColor);
void resetColor();
void clearScreen();
Key getKeyPress();

#endif
