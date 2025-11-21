#ifndef HELPERS_H
#define HELPERS_H

#include <string>

#define START_X 4
#define START_Y 2

bool isFileExist(const std::string &fileName);
bool hasInvalidChars(const std::string &fileName);
bool isInputInteger(const std::string &input);
int strToInt(const std ::string &input);
std::string getValidFilename();
int getValidFileSize();

#endif