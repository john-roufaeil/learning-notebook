#ifndef TERMINAL_H
#define TERMINAL_H

#include <string>

enum InputMode {
    CMDMODE,    // Keys are interpreted but not displayed (used for commands)
    ECHOMODE    // Keys are displayed on terminal
};

enum SpecialKey {
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
    union {
        char c;
        SpecialKey sk;
    };
};

struct TerminalSize {
    int cols;
    int rows;
};

void gotoxy(short x, short y);
int getColorCode(const std::string &color);
void setColor(const std::string &textColor, const std::string &bgColor = "none");
void resetColor();
void clearScreen();
void hideCursor(bool hide);
void setInputMode(InputMode mode);
Key getKeyPress();
TerminalSize getTerminalSize();
const char *getCurrentUsername();
void printAt(
    const std::string &text, short x, short y, 
    const std::string &textColor = "white", const std::string &bgColor = "none"
);

#endif
