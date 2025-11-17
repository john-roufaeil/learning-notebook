#include "library.h"
#include <iostream>

void newScreen() {
    clearScreen();
    textColor("cyan");
    gotoxy(4, 2);
    std::cout << "=== NEW ===";
    gotoxy(4, 4);
    textColor("reset");
    std::cout << "New page.";
    displayInstructions("Press backspace to return to homepage.", 4, 2);
    while (true) {
        Key key = getKeyPress();
        if (key == Key::BACKSPACE) {
            break;
        } else if (key == Key::ESCAPE) {
            clearScreen();
            exit(0);
        }
    }
}

void displayScreen() {
    clearScreen();
    textColor("cyan");
    gotoxy(4, 2);
    std::cout << "=== DISPLAY ===";
    gotoxy(4, 4);
    textColor("reset");
    std::cout << "Display page.";
    displayInstructions("Press backspace to return to homepage.", 4, 2);
    while (true) {
        Key key = getKeyPress();
        if (key == Key::BACKSPACE) {
            break;
        } else if (key == Key::ESCAPE) {
            clearScreen();
            exit(0);
        }
    }
}

int main() {
    Menu mainMenu("Main Menu", 4, 2);
    mainMenu.addItem("New", newScreen);
    mainMenu.addItem("Display", displayScreen);
    mainMenu.addItem("Exit", []() {
        clearScreen();
        exit(0);
    });
    mainMenu.run();
    return 0;
}
