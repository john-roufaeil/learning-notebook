#include <iostream>
#include "terminal.h"
#include "validators.h"
#include "files.h"

#define START_X 4
#define START_Y 2

void newScreen() {
    int rowWidth = getTerminalSize().cols - START_X * 2;
    clearScreen();
    write("=== Create a New File ===", START_X, START_Y, "lightmagenta");

    std::string fileName = getValidFilename();
    int count = getValidFileSize();

    write(std::string(rowWidth, '-'), START_X, START_Y + 4, "lightblue");

    // placeholder highlight for input area
    setColor("white", "lightblue");
    for (int i = 0; i < count; i++) {
        gotoxy(START_X + (i % rowWidth), START_Y + 6 + (i / rowWidth));
        std::cout << " ";
    }

    char *fileContent = (char*)malloc(count + 1);
    if (fileContent) {
        for (int i = 0; i < count; i++) {
            fileContent[i] = ' ';
        }
        fileContent[count] = '\0';
    }
    takeFileInput(count, fileContent, const_cast<char*>(fileName.c_str()));
    free(fileContent);
    fileContent = nullptr;
}

void getKeyPressInMainMenu(int menuSize, int &selectedIndex) {
    Key key = getKeyPress();
        if (key.isSpecial) {
            switch (key.sk) {
                case UP:
                case LEFT:
                    if (selectedIndex == 0) selectedIndex = menuSize - 1;
                    else selectedIndex--;
                    break;
                case DOWN:
                case RIGHT:
                    if (selectedIndex == menuSize - 1) selectedIndex = 0;
                    else selectedIndex++;
                    break;
                case HOME:  selectedIndex = 0;  break;
                case END:  selectedIndex = menuSize - 1; break;
                case ENTER:
                    switch(selectedIndex) {
                        case 0: newScreen(); break;
                        case 1: displayExistingFiles(); break;
                        case 2: clearScreen(); exit(0);
                    }
                    break;
                case ESC: clearScreen(); exit(0);
            }
        }
}

int main() {
    std::string menuItems[] = { "New File", "Display Files", "Exit" };
    std::string username = getCurrentUsername();
    int menuSize = sizeof(menuItems) / sizeof(menuItems[0]);
    int selectedIndex = 0;

    setInputMode(CMDMODE);
    while (true) {
        clearScreen();
        write("Hi, " + username + "! Welcome to MiniVi Editor", START_X, START_Y, "lightmagenta");

        for (short i = 0; i < menuSize; i++) {
            if (i == selectedIndex) {
                write("> " + menuItems[i], START_X, START_Y + 2 + i, "lightcyan");
            } else {
                write("  " + menuItems[i], START_X, START_Y + 2 + i);
            }
        }

        write("Arrow Keys, Home, End > Navigate | Enter > Select | Esc > Exit", 
            START_X, getTerminalSize().rows - START_Y, "yellow");

        getKeyPressInMainMenu(menuSize, selectedIndex);
    }

    return 0;
}