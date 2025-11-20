#ifndef TERMINAL_H
#define TERMINAL_H

#include <string>

enum InputMode
{
    CMD,    // Keys are interpreted but not displayed (used for commands)
    ECHO    // Keys are displayed on terminal
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
void setColor(std::string textColor, std::string bgColor = "transparent");
void resetColor();
void clearScreen();
void hideCursor(bool hide);
void setCursorBlinking(bool blinking);
void setInputMode(InputMode mode); // Allow or disallow user input text to be shown on terminal
Key getKeyPress();
TerminalSize getTerminalSize();
// handle window resize
// handle scroll

#endif