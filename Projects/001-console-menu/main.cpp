#include "library.h"
#include <iostream>

void newScreen() {
    clearScreen();
    changeColor("cyan");
    gotoxy(4, 2);
    std::cout << "=== NEW ===";
    resetColor();
    gotoxy(4, 4);
    std::cout << "New page.";
    while (true) {
        Key key = getKeyPress();
        if (key == BACKSPACE) {
            break;
        } else if (key == ESCAPE) {
            clearScreen();
            exit(0);
        }
    }
}

void displayScreen() {
    clearScreen();
    changeColor("cyan");
    gotoxy(4, 2);
    std::cout << "=== DISPLAY ===";
    resetColor();
    gotoxy(4, 4);
    std::cout << "Display page.";
    while (true) {
        Key key = getKeyPress();
        if (key == BACKSPACE) {
            break;
        } else if (key == ESCAPE) {
            clearScreen();
            exit(0);
        }
    }
}

int main() {
    std::string menuItems[] = { "New", "Display", "Exit" };
    int menuSize = sizeof(menuItems) / sizeof(menuItems[0]);
    int selectedIndex = 0;

    while (true) {
        clearScreen();
        changeColor("cyan");
        gotoxy(4, 2);
        std::cout << "=== MAIN MENU ===";
        resetColor();

        for (short i = 0; i < menuSize; i++) {
            gotoxy(4, 4 + i);
            if (i == selectedIndex) {
                changeColor("lightmagenta");
                std::cout << "> " << menuItems[i];
                resetColor();
            } else {
                std::cout << "  " << menuItems[i];
            }
        }

        Key key = getKeyPress();
        switch (key) {
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
            case ESCAPE:
                clearScreen();
                exit(0);
            default:
                break;
        }
    }

    return 0;
}
