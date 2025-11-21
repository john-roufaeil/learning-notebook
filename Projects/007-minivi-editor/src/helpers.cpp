#include "helpers.h"
#include "terminal.h"

bool isFileExist(const std::string* fileName) {
    std::string path = "./Files/" + *fileName;
    FILE* file = fopen(path.c_str(), "r");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}

bool hasInvalidChars(const std::string &fileName) {
    std::string forbiddenChars = "\\/:;?!\"'<>|*@#$%^&()+={}[]~,`";
    for (int i = 0; i < fileName.size(); i++) {
        for (int j = 0; j < forbiddenChars.size(); j++) {
            if (fileName[i] == forbiddenChars[j]) {
                return true;
            }
        }
    }
    return false;
}

bool isInputInteger(const std::string &input) {
    for (int i = 0; i < input.size(); i++) {
        if (input[0] == '-') {
            continue;
        }
        if (input[i] < '0' || input[i] > '9') {
            return false;
        }
    }
    return true;
}

int strToInt(const std ::string &input) {
    int num = 0;
    bool isNegative = 0;
    for (int i = 0; i < input.size(); i++)
    {
        if (input[0] == '-') {
            isNegative = 1;
            continue;       
        }
        num = (input[i] - '0') + num * 10;
    }
    return isNegative ? -num : num;
}

std::string getValidFilename() {
    std::string fileNameInput;
    std::string fileNamePrompt = "Enter File Name: ";
    bool isEmpty = false;
    bool hasInvalid = false;
    bool fileExists = false;
    gotoxy(START_X, START_Y + 2);
    std::cout<<fileNamePrompt;
    while(true) {
        setInputMode(ECHOMODE);
        std::getline(std::cin, fileNameInput);
        setInputMode(CMDMODE);
        isEmpty = fileNameInput.empty();
        fileExists = isFileExist(&fileNameInput);
        hasInvalid = hasInvalidChars(fileNameInput);
        
        if (!isEmpty && !hasInvalid && !fileExists){
            gotoxy(START_X, START_Y + 3);
            std::cout << std::string(80, ' ');
            return fileNameInput;
        }
        
        gotoxy(START_X, START_Y + 3);
        std::cout << std::string(80, ' ');
        gotoxy(START_X, START_Y + 3);
        setColor("white", "red");
        if (isEmpty)
            std::cout<<"File name cannot be empty. Please enter a valid file name.";
        else if (hasInvalid)
            std::cout<<"File name contains invalid characters. Please enter a different file name.";
        else if (fileExists)
            std::cout<<"File already exists. Please enter a different file name.";
        resetColor();
        gotoxy(START_X + fileNamePrompt.size(), START_Y + 2);
        std::cout << "                  ";
        gotoxy(START_X + fileNamePrompt.size(), START_Y + 2);
    }
}

int getValidFileSize() {
    int fileSize;
    std::string fileSizeInput;
    std::string fileSizePrompt = "Enter File Size: ";
    bool isEmpty = false;
    bool isInteger = false;
    bool isPositive = false;
    gotoxy(START_X, START_Y + 3);
    std::cout<<fileSizePrompt;
    while(true) {
        setInputMode(ECHOMODE);
        std::getline(std::cin, fileSizeInput);
        setInputMode(CMDMODE);
        isEmpty = fileSizeInput.empty();
        isInteger = isInputInteger(fileSizeInput);
        fileSize = strToInt(fileSizeInput);
        isPositive = fileSize > 0;

        if (!isEmpty && isInteger && isPositive){
            gotoxy(START_X, START_Y + 4);
            std::cout << std::string(80, ' ');
            return fileSize;
        }
        
        gotoxy(START_X, START_Y + 4);
        std::cout << std::string(80, ' ');
        gotoxy(START_X, START_Y + 4);
        setColor("white", "red");
        if (isEmpty)
            std::cout<<"File size cannot be empty. Please enter a valid file size.";
        else if (!isInteger)
            std::cout<<"File size contains invalid characters. Please enter a valid file size.";
        else if (!isPositive)
            std::cout<<"File size must be a positive integer. Please enter a valid file size.";
        resetColor();
        gotoxy(START_X + fileSizePrompt.size(), START_Y + 3);
        std::cout << "                  ";
        gotoxy(START_X + fileSizePrompt.size(), START_Y + 3);
    }
}

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
                    char emptyChar = ' ';
                    i--;
                    gotoxy(START_X + (i % rowWidth), START_Y + 6 + (i / rowWidth));
                    std::cout << " ";
                    // fileContent[i] = emptyChar;
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
                char emptyChar = ' ';
                i--;
                gotoxy(START_X + (i % rowWidth), START_Y + 6 + (i / rowWidth));
                std::cout << " ";
                // fileContent[i] = ' ';
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
                } else {
                    saveFile(fileContent, fileName);
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
