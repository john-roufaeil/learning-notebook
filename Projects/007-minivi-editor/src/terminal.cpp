#include "terminal.h"
#include "helpers.h"

void gotoxy(short x, short y) {
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

int getColorCode(const std::string &color) {
    std::string colors[] = {"black", "blue", "green", "cyan", "red", "magenta", "brown",
        "lightgray", "darkgray", "lightblue", "lightgreen", "lightcyan", "lightred", "lightmagenta", "yellow", "white"};
    int LinuxClrMap[] = {30, 34, 32, 36, 31, 35, 33, 37, 90, 94, 92, 96, 91, 95, 93, 97};
    const int numOfColors = sizeof(colors) / sizeof(colors[0]);
    for (int i = 0; i < numOfColors; i++) {
        if (color == colors[i]) {
            #ifdef _WIN32
                return i;
            #else
                return LinuxClrMap[i];
            #endif
        }
    }
    #ifdef _WIN32
        return 15;
    #else
        return 97;
    #endif
}

void setColor(const std::string &textColor, const std::string &bgColor) {
    int textColorCode = getColorCode(textColor);
#ifdef _WIN32
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    if(bgColor == "none")
        SetConsoleTextAttribute(consoleHandle, textColorCode);
    else {
        int bgColorCode = getColorCode(bgColor);
        SetConsoleTextAttribute(consoleHandle, textColorCode + bgColorCode * 16);
    }
#else
    if (bgColor == "none")
        std::cout << "\033[" << textColorCode << "m";
    else {
        int bgColorCode = getColorCode(bgColor);
        std::cout << "\033[" << textColorCode << ";" << bgColorCode + 10 << "m";
    }
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

void hideCursor(bool hide) {
#ifdef _WIN32
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    GetConsoleCursorInfo(consoleHandle, &info);
    info.bVisible = hide ? FALSE : TRUE;
    SetConsoleCursorInfo(consoleHandle, &info);
#else
    std::cout << "\e[?25" << (hide ? "l" : "h");
#endif
}

void setInputMode(InputMode mode) {
#ifdef _WIN32
    HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);
    DWORD dwMode;
    GetConsoleMode(inputHandle, &dwMode);
    if (mode == CMDMODE)  {
        dwMode &= ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
        hideCursor(true);
    }
    else {
        dwMode |= (ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
        hideCursor(false);
    }
    SetConsoleMode(inputHandle, dwMode);
#else
    termios terminal;
    tcgetattr(STDIN_FILENO, &terminal);
    if (mode == CMDMODE) {
        terminal.c_lflag &= ~(ECHO | ICANON);
        hideCursor(true);
    }
    else {
        terminal.c_lflag |= (ECHO | ICANON);
        hideCursor(false);
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &terminal);
#endif
}

Key getKeyPress() {
#ifdef _WIN32
    int ch = _getch();
    if (ch == 0 || ch == 224) {
        int ch2 = _getch();
        switch(ch2) {
            case 71: return Key{true, false, false, HOME};
            case 72: return Key{true, false, false, UP};
            case 75: return Key{true, false, false, LEFT};
            case 77: return Key{true, false, false, RIGHT};
            case 79: return Key{true, false, false, END};
            case 80: return Key{true, false, false, DOWN};
        }
    }
    switch (ch)
    {
        case 8: return Key{true, false, false, BACKSPACE};
        case 9: return Key{true, false, false, TAB};
        case 13: return Key{true, false, false, ENTER};
        case 27: return Key{true, false, false, ESC};
    }
    if (ch >= 1 && ch <= 26) // Ctrl+A - Ctrl+Z
        return Key{false, false, true, static_cast<char>(ch + 'a' - 1)};
    if (ch >= 65 && ch <= 90) // Shift+A - Shift+Z
        return Key{false, true, false, static_cast<char>(ch + 32)};
    if (ch >= 32 && ch <= 126) // Lowercase char, numbers, symbols
        return Key{false, false, false, static_cast<char>(ch)};
    return Key{true, false, false, UNKNOWN};
#else
    int ch = getchar();
    if (ch == 27) {
        int next1 = getchar();
        if (next1 == '[') {
            int next2 = getchar();
            if (next2 == '1' || next2 == '7') return Key{true, false, false, HOME};
            if (next2 == '4' || next2 == '8') return Key{true, false, false, END};
            if (next2 == 'A') return Key{true, false, false, UP};
            if (next2 == 'B') return Key{true, false, false, DOWN};
            if (next2 == 'C') return Key{true, false, false, RIGHT};
            if (next2 == 'D') return Key{true, false, false, LEFT};
            if (next2 == 'H') return Key{true, false, false, HOME};
            if (next2 == 'F') return Key{true, false, false, END};
        }
        return Key{true, false, false, ESC};
    }
    if (ch == 10) return Key{true, false, false, ENTER};
    if (ch == 127) return Key{true, false, false, BACKSPACE};
    if (ch >= 1 && ch <= 26) // Ctrl+A - Ctrl+Z
        return Key{false, false, true, static_cast<char>(ch + 'a' - 1)};
    // if (ch >= 65 && ch <= 90) // Shift+A - Shift+Z
    //     return Key{false, true, false, static_cast<char>(ch + 32)};
    if (ch >= 32 && ch <= 126) // Lowercase char, numbers, symbols
        return Key{false, false, false, static_cast<char>(ch)};
    return Key{true, false, false, UNKNOWN};
#endif
}

TerminalSize getTerminalSize() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    return TerminalSize{columns, rows};
#else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); // terminal i/o control get window size

    return TerminalSize{w.ws_col, w.ws_row};
#endif
}

// void typeText(const std::string &message, int delayTime) {
//     for (int i = 0; i < static_cast<float> (message.length()); i++) {
//         std::cout << message[i];
//         std::cout.flush();
//         delay(delayTime);
//     }
// }

void delay(int milliseconds) {
#ifdef _WIN32
    Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);
#endif
}

const char *getCurrentUsername() {
#ifdef _WIN32
    static char username[UNLEN + 1]; // UNLEN is a macro defined in <windows.h> for max username length
    DWORD size = UNLEN + 1;
    if (GetUserNameA(username, &size))
        return username;
    return "user";
#else
    char* name = getpwuid(getuid())->pw_name;
    return name ? name : "user";
#endif
}
