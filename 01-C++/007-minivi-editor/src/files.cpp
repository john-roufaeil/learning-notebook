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
    printAt("Esc > Stop Input", START_X, getTerminalSize().rows - START_Y, "yellow");

    setInputMode(CMDMODE);
    hideCursor(false);
    gotoxy(START_X, START_Y + 6);
    setColor("white", "lightblue");

    int i = 0;
    while (true) {
        Key key = getKeyPress();

        if (key.isSpecial) {
            if (key.sk == ESC) break;
            if (i > 0) {
                if (key.sk == LEFT) i--;
                else if (key.sk == UP) i = std::max(0,i - rowWidth);
                else if (key.sk == BACKSPACE) {
                    i--;
                    gotoxy(START_X + (i % rowWidth), START_Y + 6 + (i / rowWidth));
                    std::cout << " ";
                }
            } 
            if (i != count) {
                if (key.sk == RIGHT && (i < count - 1))  i++;
                else if (key.sk == DOWN && i < count) i = std::min(count - 1,i + rowWidth);
            }
            gotoxy(START_X + (i % rowWidth), START_Y + 6 + (i / rowWidth));
        } else if (i < count){
            std::cout << key.c;
            fileContent[i] = key.c;
            i++; // cursor moves when printing
            gotoxy(START_X + (i % rowWidth), START_Y + 6 + (i / rowWidth));
        }
    }
    fileContent[count] = '\0';
    resetColor();
    endFileInput(count, fileContent, fileName);
}

void endFileInput(int count, char *fileContent, char *fileName ) {
    setInputMode(CMDMODE);
    std::string savePrompt = "Would you like to save the file?";
    std::string options[2] = {" Yes ", " Cancel "};
    int selectedIndex = 0;
    printAt(savePrompt, START_X, getTerminalSize().rows - START_Y - 1, "yellow");
    printAt("Enter > Select | Arrows > Navigate | Esc > Exit", START_X, getTerminalSize().rows - START_Y, "yellow");

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
            if (key.sk == LEFT && selectedIndex > 0) selectedIndex--;
            else if (key.sk == RIGHT && selectedIndex < 1) selectedIndex++;
            else if (key.sk == ENTER) {
                if (selectedIndex == 1) takeFileInput(count, fileContent, fileName);
                else {saveFile(fileContent, fileName); break;}
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
            files[fileCount] = entry.path().filename();
            fileCount++;
        }
    }
    int selectedIndex = 0;

    while(true) {
        clearScreen();
        printAt("=== DISPLAY ===", START_X, START_Y, "cyan");
        printAt("Arrows > Navigate | Enter > Select | Backspace > Back | Esc > Exit", 
            START_X, getTerminalSize().rows - START_Y, "yellow");

        for (int i = 0; i < fileCount; i++) {
            gotoxy(START_X, START_Y + 2 + i);
            if (i == selectedIndex) printAt("> " + files[i],START_X, START_Y + 2 + i, "lightcyan");
            else printAt("  " + files[i],START_X, START_Y + 2 + i);
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
            } else if (key.sk == ENTER) viewFileContent(files[selectedIndex]);
            else if (key.sk == BACKSPACE) return;
            else if (key.sk == ESC) {
                clearScreen();
                exit(0);
            }
        }
    }
}

void viewFileContent(const std::string &filename) {
    clearScreen();
    int rowWidth = getTerminalSize().cols - 2 * START_X;
    printAt("=== " + filename + " ===", START_X, START_Y, "cyan");
    printAt("Backspace > Back | Esc > Exit", START_X,  getTerminalSize().rows - START_Y, "yellow");
    printAt(std::string(rowWidth, '-'), START_X, START_Y + 1);

    // read file
    std::string path = "./Files/" + filename;
    std::ifstream file(path);

    std::string line;
    std::getline(file, line);
    for (int i = 0; i < line.length(); i++) {
        gotoxy(START_X + (i % rowWidth), START_Y + 3 + (i / rowWidth));
        std::cout << line[i];
    }
    
    while (true) {
        Key key = getKeyPress();
        if (key.isSpecial && key.sk == BACKSPACE) return;
        if (key.isSpecial && key.sk == ESC) {
            clearScreen();
            exit(0);
        }
    }
}
