#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <fstream>

#define START_X 4
#define START_Y 2

bool isFileExist(const std::string *fileName);
bool hasInvalidChars(const std::string &fileName);
bool isInputInteger(const std::string &input);
int strToInt(const std ::string &input);
std::string getValidFilename();
int getValidFileSize();
void takeFileInput(int count, char *fileContent,char *fileName);
void endFileInput(int count, char *fileContent, char *fileName);
void saveFile(char *fileContent, char *fileName);
void displayExistingFiles();
void viewFileContent(const std::string &filename);

#endif