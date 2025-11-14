
#include "library.h"
#include <iostream>
#include <map>
#include <cstdio>

#ifdef _WIN32
    #include <windows.h>
#endif

void gotoxy(int x, int y) {
    #ifdef _WIN32
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    #else
        // ANSI escape sequence for Unix/Linux/Mac
        printf("\033[%d;%dH", y + 1, x + 1);
        fflush(stdout);
    #endif
}

void textColor(const std::string& color) {
    #ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        static std::map<std::string, int> colors = {
            {"black", 0}, {"blue", 1}, {"green", 2}, {"cyan", 3},
            {"red", 4}, {"magenta", 5}, {"yellow", 6}, {"white", 7},
            {"reset", 7}
        };

        auto it = colors.find(color);
        if (it != colors.end()) {
            SetConsoleTextAttribute(hConsole, it->second);
        }
    #else
        // ANSI escape codes for Unix/Linux/Mac
        static std::map<std::string, std::string> colors = {
            {"black", "\033[30m"}, {"red", "\033[31m"},
            {"green", "\033[32m"}, {"yellow", "\033[33m"},
            {"blue", "\033[34m"}, {"magenta", "\033[35m"},
            {"cyan", "\033[36m"}, {"white", "\033[37m"},
            {"reset", "\033[0m"}
        };

        auto it = colors.find(color);
        if (it != colors.end()) {
            printf("%s", it->second.c_str());
            fflush(stdout);
        }
    #endif
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
