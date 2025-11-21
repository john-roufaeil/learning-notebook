#include "terminal.h"
#include "helpers.h"
#include "files.h"

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
    int rowWidth = getTerminalSize().cols - 2 * START_X;
    clearScreen();
    setColor("lightmagenta");
    gotoxy(START_X, START_Y);
    std::cout << "=== Create a New File ===";
    resetColor();

    std::string fileName = getValidFilename();
    int count = getValidFileSize();

    gotoxy(START_X, START_Y + 4);
    setColor("lightblue");
    std::cout << std::string(getTerminalSize().cols - START_X * 2, '-');

    // placeholder highlight for input area
    gotoxy(START_X, START_Y + 6);
    setColor("white", "lightblue");
    for (int i = 0; i < count; i++) {
        gotoxy(START_X + (i % rowWidth), START_Y + 6 + (i / rowWidth));
        std::cout << " ";
    }

    char *fileContent = (char *)malloc(sizeof(char) * (count + 1));
    if (fileContent) {
        for (int i = 0; i < count; i++) {
            fileContent[i] = ' ';
        }
    }
    takeFileInput(count, fileContent, const_cast<char*>(fileName.c_str()));
    free(fileContent);
    // waitForBackOrExit();
}

void displayScreen() {
    displayExistingFiles();
}

void mainMenu() {
    setInputMode(CMDMODE);
    std::string menuItems[] = { "New File", "Display Files", "Exit" };
    int menuSize = sizeof(menuItems) / sizeof(menuItems[0]);
    int selectedIndex = 0;
    while (true) {
        clearScreen();
        // header
        setColor("lightmagenta");
        gotoxy(START_X, START_Y);
        std::cout << "Hi, " << getCurrentUsername() << "! Welcome to MiniVi Editor" << std::endl;
        resetColor();

        // menu
        for (short i = 0; i < menuSize; i++) {
            gotoxy(START_X, START_Y + 2 + i);
            if (i == selectedIndex) {
                setColor("lightcyan");
                std::cout << "> " << menuItems[i];
                resetColor();
            } else {
                std::cout << "  " << menuItems[i];
            }
        }
        resetColor();

        // instructions
        gotoxy(START_X, getTerminalSize().rows - START_Y);
        setColor("yellow");
        std::cout << "Arrow Keys, Home, End > Navigate | Enter > Select | Esc > Exit";
        resetColor();

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
                    if (selectedIndex == 0) {
                        newScreen();
                    } else if (selectedIndex == 1) {
                        displayScreen();
                    } else if (selectedIndex ==  2) {
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
}

int main() {
    mainMenu();
    return 0;
}
