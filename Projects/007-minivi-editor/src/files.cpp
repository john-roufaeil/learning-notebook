#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>
#include "files.h"
#include "terminal.h"

#define START_X 4
#define START_Y 2
#define MAX_NUMBER_OF_FILES 100


void takeFileInput(int count, char *fileContent, char *fileName) {
    int rowWidth = getTerminalSize().cols - 2 * START_X;
    resetColor();
    gotoxy(START_X, getTerminalSize().rows - START_Y - 1);
    std::cout << std::string(rowWidth, ' ');
    gotoxy(START_X, getTerminalSize().rows - START_Y);
    std::cout << std::string(rowWidth, ' ');
    gotoxy(START_X, getTerminalSize().rows - START_Y);
    setColor("yellow");
    std::cout << "Esc > Stop Input";
    resetColor();

    setInputMode(CMDMODE);
    hideCursor(false);
    gotoxy(START_X, START_Y + 6);
    setColor("white", "lightblue"); 

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
            }

            if (key.sk == RIGHT && (i < count - 1)) {
                i++;
                gotoxy(START_X + (i % rowWidth), START_Y + 6 + (i / rowWidth));
            }

            if (key.sk == UP && i > 0) {
                i -= rowWidth;
                if (i < 0) { i = 0;
                }
                gotoxy(START_X + (i % rowWidth), START_Y + 6 + (i / rowWidth));
            }

            if (key.sk == DOWN && i < count) {
                i += rowWidth;
                if (i > count - 1) {
                    i = count - 1;
                }
                gotoxy(START_X + (i % rowWidth), START_Y + 6 + (i / rowWidth));
            }

            if (key.sk == BACKSPACE && i > 0) {
                i--;
                gotoxy(START_X + (i % rowWidth), START_Y + 6 + (i / rowWidth));
                std::cout << " ";
                gotoxy(START_X + (i % rowWidth), START_Y + 6 + (i / rowWidth));
            }

            if (key.sk == ESC) break;

            continue;
        }
        
        gotoxy(START_X + (i % rowWidth), START_Y + 6 + (i / rowWidth));
        std::cout << key.c;
        fileContent[i] = key.c;
        i++; // cursor moves when printing
    }
    fileContent[count] = '\0';

    endFileInput(count, fileContent, fileName);
}

void endFileInput(int count, char *fileContent, char *fileName ) {
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
                if (selectedIndex == 1) {
                    takeFileInput(count, fileContent, fileName);
                    return;
                }
                else
                {
                    saveFile(fileContent, fileName);
                    return;
                }
            } else if (key.sk == ESC) {
                clearScreen();
                exit(0);
            }
        }
    }
    resetColor();
}

void saveFile(char *fileContent, char *fileName) {
    std::string path = "./Files/" + std::string(fileName);
    FILE *file = fopen(path.c_str(), "w");
    if (file) {
        fputs(fileContent, file);
        fclose(file);
    }
}

void displayExistingFiles() {
    std::string files[MAX_NUMBER_OF_FILES];
    std::string path = "./Files";
    std::filesystem::directory_iterator it = std::filesystem::directory_iterator(path);
    std::filesystem::directory_iterator end = std::filesystem::directory_iterator();
    
    int fileCount = 0;
    for (; it != end; it++) {
        std::filesystem::directory_entry entry = *it;
        if (std::filesystem::is_regular_file(entry.path())) {
            files[fileCount] = entry.path().filename().string();
            fileCount++;
        }
    }
    int selectedIndex = 0;

    while(true) {
        clearScreen();
        setColor("cyan");
        gotoxy(START_X, START_Y);
        std::cout << "=== DISPLAY ===";
        resetColor();
    
        gotoxy(START_X, getTerminalSize().rows - START_Y);
        setColor("yellow");
        std::cout << "Arrows > Navigate | Enter > Select | Backspace > Back | Esc > Exit";
        resetColor();
        for (int i = 0; i < fileCount; i++)
        {
            gotoxy(START_X, START_Y + 2 + i);
            if (i == selectedIndex) {
                setColor("lightcyan");
                std::cout << "> " << files[i];
                resetColor();
            } else {
                std::cout << "  " << files[i];
            }
        }

        Key key = getKeyPress();
        if (key.isSpecial) {
            if (key.sk == UP) {
                selectedIndex--;
                if (selectedIndex < 0)
                    selectedIndex = fileCount - 1;
            } else if (key.sk == DOWN) {
                selectedIndex++;
                if (selectedIndex == fileCount)
                    selectedIndex = 0;
            } else if (key.sk == ENTER) {
                viewFileContent(files[selectedIndex]);
            } else if (key.sk == BACKSPACE) {
                return;
            } else if (key.sk == ESC) {
                clearScreen();
                exit(0);
            }
        }
    }
}

void viewFileContent(const std::string &filename) {
    clearScreen();
    gotoxy(START_X, START_Y);
    setColor("cyan");
    std::cout << "=== " << filename << " ===";
    gotoxy(START_X, getTerminalSize().rows - START_Y);
    setColor("yellow");
    std::cout << "Backspace > Back | Esc > Exit";
    resetColor();
    gotoxy(START_X, START_Y + 1);
    std::cout << std::string(getTerminalSize().cols - 2 * START_X, '-');
    resetColor();

    // read file
    std::string path = "./Files/" + filename;
    std::ifstream file(path);

    int rowWidth = getTerminalSize().cols - 2 * START_X;

    std::string line;
    std::getline(file, line);
    for (int i = 0; i < line.length(); i++) {
        gotoxy(START_X + (i % rowWidth), START_Y + 3 + (i / rowWidth));
        std::cout << line[i];
    }
    
    while (true) {
        Key key = getKeyPress();
        if (key.isSpecial && key.sk == BACKSPACE)
            return;
        if (key.isSpecial && key.sk == ESC) {
            clearScreen();
            exit(0);
        }
    }
}
