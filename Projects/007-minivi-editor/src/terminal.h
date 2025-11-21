#ifndef TERMINAL_H
#define TERMINAL_H

#include <string>
#include <iostream>
#include <filesystem>

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
    #include <Lmcons.h>
#else
    #include <unistd.h>
    #include <termios.h>
    #include <sys/ioctl.h>
    #include <pwd.h>
#endif

enum InputMode
{
    CMDMODE,    // Keys are interpreted but not displayed (used for commands)
    ECHOMODE    // Keys are displayed on terminal
};

enum SpecialKey
{
    UP,
    DOWN,
    RIGHT,
    LEFT,
    ESC,
    ENTER,
    TAB,
    HOME,
    END,
    BACKSPACE,
    UNKNOWN
};

struct Key {
    bool isSpecial;
    bool isShift;
    bool isCtrl;
    union
    {
        char c;
        SpecialKey sk;
    };
};

struct TerminalSize
{
    int cols;
    int rows;
};

void gotoxy(short x, short y);
int getColorCode(const std::string &color);
void setColor(const std::string &textColor, const std::string &bgColor = "none");
void resetColor();
void clearScreen();
void hideCursor(bool hide);
void setInputMode(InputMode mode); // Allow or disallow user input text to be shown on terminal
Key getKeyPress();
TerminalSize getTerminalSize();
void delay(int milliseconds);
// void typeText(const std::string &message, int delayTime = 10);
const char *getCurrentUsername();

#endif
