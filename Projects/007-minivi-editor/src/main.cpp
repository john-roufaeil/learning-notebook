#include "terminal.h"

#define START_X 4
#define START_Y 2

void waitForBackOrExit() {
    while (true) {
        Key key = getKeyPress();
        if (key.isSpecial && key.sk == BACKSPACE) break;
        if (key.isSpecial && key.sk == ESC) {
            clearScreen();
            exit(0);
        }
    }
}

void newScreen() {
    clearScreen();
    setColor("cyan");
    gotoxy(START_X, START_Y);
    std::cout << "=== NEW ===";
    resetColor();
    gotoxy(START_X, START_Y + 2);
    std::cout << "New page.";
    waitForBackOrExit();
}

void displayScreen() {
    clearScreen();
    setColor("cyan");
    gotoxy(START_X, START_Y);
    std::cout << "=== DISPLAY ===";
    resetColor();
    gotoxy(START_X, START_Y + 2);
    std::cout << "Display page.";
    waitForBackOrExit();
}

int main() {
    std::string menuItems[] = { "New", "Display", "Exit" };
    int menuSize = sizeof(menuItems) / sizeof(menuItems[0]);
    int selectedIndex = 0;

    while (true) {
        hideCursor(true);
        clearScreen();
        setColor("cyan");
        gotoxy(START_X, START_Y);
        std::cout << "=== MAIN MENU ===";
        resetColor();

        for (short i = 0; i < menuSize; i++) {
            gotoxy(START_X, START_Y + 2 + i);
            if (i == selectedIndex) {
                setColor("lightmagenta");
                std::cout << "> " << menuItems[i];
                resetColor();
            } else {
                std::cout << "  " << menuItems[i];
            }
        }

        Key key = getKeyPress();
        if (key.isSpecial)
            switch (key.sk) {
                case UP:
                case LEFT:
                    if (selectedIndex == 0) selectedIndex = menuSize - 1;
                    else selectedIndex--;
                    break;
                case DOWN:
                case RIGHT:
                    if (selectedIndex == menuSize - 1)
                        selectedIndex = 0;
                    else selectedIndex++;
                    break;
                case HOME:
                    selectedIndex = 0;
                    break;
                case END:
                    selectedIndex = menuSize - 1;
                    break;
                case ENTER:
                    if (menuItems[selectedIndex] == "New") {
                        newScreen();
                    } else if (menuItems[selectedIndex] == "Display") {
                        displayScreen();
                    } else if (menuItems[selectedIndex] == "Exit") {
                        clearScreen();
                        exit(0);
                    }
                    break;
                case ESC:
                    clearScreen();
                    exit(0);
                default:
                    break;
            }
    }

    return 0;
}
