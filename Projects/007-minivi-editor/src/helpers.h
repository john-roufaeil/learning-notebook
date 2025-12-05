#ifndef HELPERS_H
#define HELPERS_H

#include <string>

bool isFileExist(const std::string *fileName);
bool hasInvalidChars(const std::string &fileName);
bool isEmpty(const std::string &fileName);
bool isInputInteger(const std::string &input);
int strToInt(const std ::string &input);
std::string getValidFilename();
int getValidFileSize();

#endif