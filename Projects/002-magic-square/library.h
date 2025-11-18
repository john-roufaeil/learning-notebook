#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>
#include <vector>
#include <functional>

// Supported colors: "black", "red", "green", "yellow", "blue", "magenta", "cyan", "white", "reset"
void textColor(const std::string& color);
void gotoxy(int x, int y);
void clearScreen();
void getTerminalSize(int& width, int& height);
void displayInstructions(const std::string instructionsMessage, int x, int y);
void delay(int milliseconds);
void typeText(std::string message, int delayTime = 50);

// Keyboard input functions
enum class Key {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    ENTER,
    ESCAPE,
    BACKSPACE,
    HOME,
    END,
    UNKNOWN
};
Key getKeyPress();

// Menu system
struct MenuItem {
    std::string text;
    std::function<void()> action;
};

class Menu {
private:
    std::vector<MenuItem> items;
    int selectedIndex;
    int x, y; // Menu position
    std::string title;

public:
    Menu(const std::string& menuTitle, int posX = 5, int posY = 3);

    void addItem(const std::string& text, std::function<void()> action);
    void display();
    void run();
    int getSelectedIndex() const { return selectedIndex; }
};

#endif
