#include "library.h"
#include <iostream>
#include <map>
#include <cstdio>
#include <chrono>
#include <thread>

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <sys/select.h>
    #include <sys/ioctl.h>
#endif

void delay(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void gotoxy(int x, int y) {
#ifdef _WIN32
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
#else
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
    static std::map<std::string, std::string> colors = {
        {"black", "\033[30m"}, {"red", "\033[31m"}, {"green", "\033[32m"},
        {"yellow", "\033[33m"}, {"blue", "\033[34m"}, {"magenta", "\033[35m"},
        {"cyan", "\033[36m"}, {"white", "\033[37m"}, {"reset", "\033[0m"}
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

void getTerminalSize(int& width, int& height) {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    width = w.ws_col;
    height = w.ws_row;
#endif
}

// Keyboard input implementation
Key getKeyPress() {
#ifdef _WIN32
    int ch = _getch();

    if (ch == 0 || ch == 224) { // Special key prefix
        ch = _getch();
        switch (ch) {
            case 72: return Key::UP;
            case 80: return Key::DOWN;
            case 75: return Key::LEFT;
            case 77: return Key::RIGHT;
            case 71: return Key::HOME;
            case 79: return Key::END;
        }
    }

    switch (ch) {
        case 13: return Key::ENTER;
        case 27: return Key::ESCAPE;
        case 8: return Key::BACKSPACE;
        default: return Key::UNKNOWN;
    }
#else
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int ch = getchar();

    if (ch == 27) { // ESC or arrow key
        ch = getchar();
        if (ch == '[') {
            ch = getchar();
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            switch (ch) {
                case 'A': return Key::UP;
                case 'B': return Key::DOWN;
                case 'C': return Key::RIGHT;
                case 'D': return Key::LEFT;
                case 'H': return Key::HOME;
                case 'F': return Key::END;
                case '1': // Some terminals use different sequences
                case '7':
                    getchar(); // Consume the '~'
                    return Key::HOME;
                case '4':
                case '8':
                    getchar(); // Consume the '~'
                    return Key::END;
            }
        }
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return Key::ESCAPE;
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    switch (ch) {
        case 10: return Key::ENTER;
        case 127: return Key::BACKSPACE;
        default: return Key::UNKNOWN;
    }
#endif
}

void displayInstructions(const std::string instructionsMessage, int x, int y) {
    int width, height;
    getTerminalSize(width, height);
    gotoxy(x, height - y);
    textColor("yellow");
    std::cout << instructionsMessage;
    textColor("reset");
}

// Menu implementation
Menu::Menu(const std::string& menuTitle, int posX, int posY)
    : title(menuTitle), x(posX), y(posY), selectedIndex(0) {}

void Menu::addItem(const std::string& text, std::function<void()> action) {
    items.push_back({text, action});
}

void Menu::display() {
    // Display title
    gotoxy(x, y);
    textColor("cyan");
    std::cout << "=== " << title << " ===" << std::endl;
    textColor("reset");

    // Display menu items
    for (size_t i = 0; i < items.size(); ++i) {
        gotoxy(x, y + i + 2);

        if (i == selectedIndex) {
            textColor("green");
            std::cout << "> " << items[i].text;
            textColor("reset");
        } else {
            std::cout << "  " << items[i].text;
        }
    }

    displayInstructions("Use Arrow Keys to navigate | Home goes to top | End goes to end", x, y +1);
    displayInstructions("Enter to select | Backspace to go back | ESC to exit", x, y);
}

void Menu::run() {
    clearScreen();

    while (true) {
        display();
        Key key = getKeyPress();

        switch (key) {
            case Key::HOME:
                selectedIndex = 0;
                break;

            case Key::END:
                selectedIndex = items.size() - 1;
                break;

            case Key::UP:
                if (selectedIndex > 0) {
                    selectedIndex--;
                } else if (selectedIndex == 0) {
                    selectedIndex = items.size() - 1;
                }
                break;

            case Key::DOWN:
                if (selectedIndex < static_cast<int>(items.size()) - 1) {
                    selectedIndex++;
                } else if (selectedIndex = items.size() - 1) {
                    selectedIndex = 0;
                }
                break;

            case Key::ENTER:
                clearScreen();
                if (items[selectedIndex].action) {
                    items[selectedIndex].action();
                }
                clearScreen();
                break;

            case Key::ESCAPE:
                clearScreen();
                exit(0);
                break;

            case Key::BACKSPACE:
                return;

            default:
                break;
        }
    }
}
