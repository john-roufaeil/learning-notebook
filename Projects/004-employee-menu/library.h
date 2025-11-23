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

bool isInt(const std::string &input);
bool isAlpha(const std::string &input);
bool isValidDOB(const std::string &input);
DOB extractDOB(std::string input);
void gotoxy(int x, int y);
int getColorCode(const std::string &color);
void changeColor(const std::string &textColor);
void resetColor();
void clearScreen();
Key getKeyPress();

#endif
