#ifndef FILES_H
#define FILES_H

#define MAX_NUMBER_OF_FILES 100

#include <string>
#include <fstream>

void takeFileInput(int count, char *fileContent,char *fileName);
void endFileInput(int count, char *fileContent, char *fileName);
void saveFile(char *fileContent, char *fileName);
void displayExistingFiles();
void viewFileContent(const std::string &filename);
#endif