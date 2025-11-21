#include "terminal.h"
#include "helpers.h"

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
    setColor("lightmagenta");
    gotoxy(START_X, START_Y);
    std::cout << "=== Create a New File ===";
    resetColor();
    
    std::string fileName = getValidFilename();
    int count = getValidFileSize();

    gotoxy(START_X, START_Y + 4);
    setColor("lightblue");
    std::cout << std::string(getTerminalSize().cols - START_X * 2, '-');

    gotoxy(START_X, getTerminalSize().rows - START_Y);
    setColor("yellow");
    std::cout << "Esc > Stop Input";

    gotoxy(START_X, START_Y + 6);
    setColor("white", "lightblue"); 

    // placeholder highlight for input area
    for (int i = 0; i < count; i++) {
        if (i % (getTerminalSize().cols - 8) == 0) {
            gotoxy(START_X, START_Y + 6 + i / (getTerminalSize().cols - 8));
        }
        std::cout << " " ;
    }

    // take input
    setInputMode(CMDMODE);
    hideCursor(false);
    gotoxy(START_X, START_Y + 6);
    int rowWidth = getTerminalSize().cols - 2 * START_X;
    
    for (int i = 0; i < count + 1;) {
        Key key = getKeyPress();

        // if reached end of input area
        if (i == count && i > 0) {
            if (key.isSpecial) {
                if (key.sk == LEFT) {
                    i--;
                    gotoxy(START_X + (i % rowWidth), START_Y + 6 + (i / rowWidth));
                }

                if (key.sk == UP) {
                    i = std::max(0,i - rowWidth);
                    gotoxy(START_X + (i % rowWidth), START_Y + 6 + (i / rowWidth));
                }

                else if (key.sk == BACKSPACE) {
                    i--;
                    gotoxy(START_X + (i % rowWidth), START_Y + 6 + (i / rowWidth));
                    std::cout << " ";
                    gotoxy(START_X + (i % rowWidth), START_Y + 6 + (i / rowWidth));
                }

                else if (key.sk == ESC) break;
            }

            continue; 
        }

        if (key.isSpecial) {
            if (key.sk == LEFT && i > 0) {
                i--;
                gotoxy(START_X + (i % rowWidth), START_Y + 6 + (i / rowWidth));
                continue;
            }

            if (key.sk == RIGHT && (i < count - 1)) {
                i++;
                gotoxy(START_X + (i % rowWidth), START_Y + 6 + (i / rowWidth));
                continue;
            }

            if (key.sk == UP && i > 0) {
                i -= rowWidth;
                if (i < 0) { i = 0;
                }
                gotoxy(START_X + (i % rowWidth), START_Y + 6 + (i / rowWidth));
                continue;
            }

            if (key.sk == DOWN && i < count) {
                i += rowWidth;
                if (i > count - 1) {
                    i = count - 1;
                }
                gotoxy(START_X + (i % rowWidth), START_Y + 6 + (i / rowWidth));
                continue;
            }

            if (key.sk == BACKSPACE && i > 0) {
                i--;
                gotoxy(START_X + (i % rowWidth), START_Y + 6 + (i / rowWidth));
                std::cout << " ";
                gotoxy(START_X + (i % rowWidth), START_Y + 6 + (i / rowWidth));
                continue;
            }

            if (key.sk == ESC) break;

            continue;
        }
        
        gotoxy(START_X + (i % rowWidth), START_Y + 6 + (i / rowWidth));
        std::cout << key.c;
        i++; // cursor moves when printing
    }

    setInputMode(CMDMODE);
    resetColor();
    gotoxy(START_X, getTerminalSize().rows - START_Y);
    setColor("yellow");
    std::cout << "Enter > Select | Arrows > Navigate | Esc > Exit";
    resetColor();
    gotoxy(START_X, getTerminalSize().rows - START_Y - 1);
    std::string savePrompt = "Would you like to save the file?";
    std::cout << savePrompt;
    std::string options[2] = {" Yes ", " Cancel "};
    int selectedIndex = 0;
    while (true) {
        gotoxy(START_X * 2 + savePrompt.length(), getTerminalSize().rows - START_Y - 1);
        if (selectedIndex == 0) {
            setColor("white", "green");
            std::cout << options[0];
            resetColor();
            std::cout << "    " << options[1];
        } else {
            std::cout << options[0] << "    ";
            setColor("white", "red");
            std::cout << options[1];
            resetColor();
        }

        Key key = getKeyPress();
        if (key.isSpecial) {
            if (key.sk == LEFT && selectedIndex > 0) {
                selectedIndex--;
            } else if (key.sk == RIGHT && selectedIndex < 1) {
                selectedIndex++;
            } else if (key.sk == ENTER) {
                break;
            } else if (key.sk == ESC) {
                clearScreen();
                exit(0);
            }
        }
    }
    resetColor();

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
    
        // instructions
        gotoxy(START_X, getTerminalSize().rows - START_Y);
        setColor("yellow");
        std::cout << "Arrow Keys, Home, End > Navigate | Enter > Select | Esc > Exit";
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
