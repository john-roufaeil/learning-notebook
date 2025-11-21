#ifndef FILES_H
#define FILES_H

#include <string>
#include <fstream>

void takeFileInput(int count, char *fileContent,char *fileName);
void endFileInput(int count, char *fileContent, char *fileName);
void saveFile(char *fileContent, char *fileName);
void displayExistingFiles();
void viewFileContent(const std::string &filename);
#endif