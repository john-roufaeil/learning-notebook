#include "library.h"

void gotoxy(int x, int y) {
#ifdef _WIN32
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position;
    position.X = x;
    position.Y = y;
    SetConsoleCursorPosition(consoleHandle, position);
#else
    std::cout << "\033[" << y+1 << ";" << x+1 << "H";
#endif
}

int getColorCode(std::string color) {
    #ifdef _WIN32
        if (color == "black") return 0;
        if (color == "blue") return 1;
        if (color == "green") return 2;
        if (color == "cyan") return 3;
        if (color == "red") return 4;
        if (color == "magenta")return 5;
        if (color == "brown") return 6;
        if (color == "lightgray") return 7;
        if (color == "darkgray") return 8;
        if (color == "lightblue") return 9;
        if (color == "lightgreen") return 10;
        if (color == "lightcyan") return 11;
        if (color == "lightred") return 12;
        if (color == "lightmagenta") return 13;
        if (color == "yellow") return 14;
        return 15;
    #else
        if (color == "black") return 30;
        if (color == "red") return 31;
        if (color == "green") return 32;
        if (color == "yellow") return 33;
        if (color == "blue") return 34;
        if (color == "magenta") return 35;
        if (color == "cyan") return 36;
        if (color == "lightgray") return 37;
        if (color == "darkgray") return 90;
        if (color == "lightred") return 91;
        if (color == "lightgreen") return 92;
        if (color == "lightyellow") return 93;
        if (color == "lightblue") return 94;
        if (color == "lightmagenta") return 95;
        if (color == "lightcyan") return 96;
        return 97;
    #endif
}

void changeColor(std::string textColor) {
    int textColorCode = getColorCode(textColor);
#ifdef _WIN32
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(consoleHandle, textColorCode);
#else
    std::cout << "\033[" << textColorCode << "m";
#endif
}

void resetColor() {
#ifdef _WIN32
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(consoleHandle, 7);
#else
    std::cout << "\033[0m";
#endif
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

Key getKeyPress() {
#ifdef _WIN32
    int ch = _getch();
    if (ch == 0 || ch == 224) {
        int ch2 = _getch();
        switch(ch2) {
            case 72: return UP;
            case 80: return DOWN;
            case 75: return LEFT;
            case 77: return RIGHT;
            case 71: return HOME;
            case 79: return END;
        }
    }
    switch (ch) {
        case 13: return ENTER;
        case 27: return ESCAPE;
        case 8: return BACKSPACE;
        default: return UNKNOWN;
    }
#else
    termios originalTerminal, newTerminal;
    tcgetattr(STDIN_FILENO, &originalTerminal);
    newTerminal = originalTerminal;
    newTerminal.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newTerminal);
    int ch = getchar();
    if (ch == 27) {
        int next1 = getchar();
        if (next1 == '[') {
            int next2 = getchar();
            if (next2 == '1' || next2 == '7') return HOME;
            if (next2 == '4' || next2 == '8') return END;
            if (next2 == 'A') return UP;
            if (next2 == 'B') return DOWN;
            if (next2 == 'C') return RIGHT;
            if (next2 == 'D') return LEFT;
            if (next2 == 'H') return HOME;
            if (next2 == 'F') return END;
        }
        return ESCAPE;
    }
    if (ch == 10) return ENTER;
    if (ch == 127) return BACKSPACE;
    return UNKNOWN;
#endif
}
