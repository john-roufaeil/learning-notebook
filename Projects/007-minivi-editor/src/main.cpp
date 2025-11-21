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


    setInputMode(CMDMODE);
    typeText("Enter file name: ", 50);
    hideCursor(false);
    setInputMode(ECHOMODE);
    std::string fileNameInput;

    while(true) {
        std::getline(std::cin, fileNameInput);
        if (!fileNameInput.empty()) {
            break;
        }
        gotoxy(START_X, START_Y + 3);
        setColor("white", "red");
        typeText("File name cannot be empty. Please enter a valid file name.", 50);
        resetColor();
        gotoxy(START_X + 17, START_Y + 2);
        std::cout << "                            ";
        gotoxy(START_X + 17, START_Y + 2);
    }



    setInputMode(CMDMODE);
    typeText("Enter how many characters you will enter: ", 50);
    hideCursor(false);
    setInputMode(ECHOMODE);
    
    int count;
    std::string input;

    while (true) {
        bool valid = true;
        std::getline(std::cin, input);
        if (!input.empty()) {
            for (int i = 0; i < input.size(); i++) {
                if (!isdigit(input[i])) {
                    valid = false;
                    break;
                }
            }
        }
        if (valid) {
            count = static_cast<int>(std::stoi(input));
            gotoxy(START_X, START_Y + 3);
            std::cout << "                                                                                             ";
            gotoxy(START_X, START_Y + 3);
            setColor("white", "green"); 
            typeText("You are allowed to input up to " + std::to_string(count) + " characters.", 50);
            resetColor();
            break;
        }
        gotoxy(START_X, START_Y + 3);
        setColor("white", "red");
        typeText("Invalid input, please enter a positive integer.", 50);
        resetColor();
        gotoxy(START_X + 42, START_Y + 2);
        std::cout << "                            ";
        gotoxy(START_X + 42, START_Y + 2);
    }

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
    setInputMode(CMDMODE);
    hideCursor(true);
    clearScreen();
    setColor("lightmagenta");
    gotoxy(START_X, START_Y);
    std::cout << "Hi, " << getCurrentUsername() << "! Welcome to MiniVi Editor" << std::endl;
    resetColor();

    while (true)
    {
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
