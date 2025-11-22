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
        std::cin >> fileNameInput;
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
