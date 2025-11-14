#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>

// Supported colors: "black", "red", "green", "yellow", "blue", "magenta", "cyan", "white", "reset"
void textColor(const std::string& color);
void gotoxy(int x, int y);
void clearScreen();

#endif
