#include <iostream>
#include "validators.h"
#include "terminal.h"

#define START_X 4
#define START_Y 2

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
    std::string invalidChars = "\\/:;?!\"'<>|*@#$%^&()+={}[]~,`";
    for (int i = 0; i < fileName.size(); i++) {
        for (int j = 0; j < invalidChars.size(); j++) {
            if (fileName[i] == invalidChars[j]) return true;
        }
    }
    return false;
}

bool isEmpty(const std::string &fileName) {
    for (int i = 0; i < fileName.size(); i++) {
        if (fileName[i] != ' ' && fileName[i] != '\0') return false;
    }
    return true;
}

bool isInputInteger(const std::string &input) {
    for (int i = 0; i < input.size(); i++) {
        if (input[0] == '-') continue;
        if (input[i] < '0' || input[i] > '9') return false;
    }
    return true;
}

int strToInt(const std::string &input) {
    int num = 0;
    bool isNegative = 0;
    for (int i = 0; i < input.size(); i++) {
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
    printAt(fileNamePrompt, START_X, START_Y + 2);
    while (true) {
        setInputMode(ECHOMODE);
        std::getline(std::cin, fileNameInput);
        setInputMode(CMDMODE);
        bool empty = isEmpty(fileNameInput);
        bool fileExists = isFileExist(&fileNameInput);
        bool hasInvalid = hasInvalidChars(fileNameInput);

        gotoxy(START_X, START_Y + 3);
        std::cout << std::string(80, ' ');
        
        if (!empty && !hasInvalid && !fileExists) return fileNameInput;
        else if (empty) printAt("File name cannot be empty.", START_X, START_Y + 3, "white", "red");
        else if (hasInvalid) printAt("File name contains invalid characters.", START_X, START_Y + 3, "white", "red");
        else if (fileExists) printAt("File already exists.", START_X, START_Y + 3, "white", "red");

        gotoxy(START_X + fileNamePrompt.size(), START_Y + 2);
        std::cout << "                  ";
        gotoxy(START_X + fileNamePrompt.size(), START_Y + 2);
    }
}

int getValidFileSize() {
    std::string fileSizeInput;
    std::string fileSizePrompt = "Enter File Size: ";
    gotoxy(START_X, START_Y + 3);
    std::cout<<fileSizePrompt;
    while(true) {
        setInputMode(ECHOMODE);
        std::getline(std::cin, fileSizeInput);
        setInputMode(CMDMODE);
        bool empty = isEmpty(fileSizeInput);
        bool isInteger = isInputInteger(fileSizeInput);
        int fileSize = strToInt(fileSizeInput);
        bool isPositive = fileSize > 0;

        gotoxy(START_X, START_Y + 4);
        std::cout << std::string(80, ' ');
        
        if (!empty && isInteger && isPositive && fileSize < 500) return fileSize;
        else if (empty) printAt("File size cannot be empty.", START_X, START_Y + 4, "white", "red");
        else if (!isInteger) printAt("File size contains invalid characters.", START_X, START_Y + 4, "white", "red");
        else if (!isPositive) printAt("File size must be a positive integer.", START_X, START_Y + 4, "white", "red"); 
        else if (fileSize >= 500) printAt("File size must be less than 500.", START_X, START_Y + 4, "white", "red");

        gotoxy(START_X + fileSizePrompt.size(), START_Y + 3);
        std::cout << "                  ";
        gotoxy(START_X + fileSizePrompt.size(), START_Y + 3);
    }
}
