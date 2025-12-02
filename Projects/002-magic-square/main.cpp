#include <iostream>
#include "library.h"

void buildMagicSquareBoxes(int n, int x, int y) {
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j < n; j++) {
            gotoxy(x + j * 4, y + i * 2);
            typeText("+---", 10);
        }
        typeText("+", 10);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= n; j++) {
            gotoxy(x + j * 4, y + i * 2 + 1);
            typeText("|   ", 10);
        }
    }
}

void populateMagicSquare(int n, int x, int y) {
    int row = 0;
    int col = n / 2;
    
    for (int i = 1; i <= n * n; i++) {
        delay(250);

        gotoxy(x + col * 4 + (i >= 100 ? 1 : 2), y + row * 2 + 1);
        typeText(std::to_string(i), 50);
        
        if (i % n == 0) {
            row = (row + 1) % n;
        } else {
            row = (row - 1 + n) % n;
            col = (col - 1 + n) % n;
        }
    }
}

void takeValidInput(int &n) {
    gotoxy(4, 4);
    typeText("Enter an odd number for the size of the magic square: ", 50);
    std::cin >> n;
    while (n % 2 == 0 || n < 3 || n > 31) {
        gotoxy(4, 5);
        changeColor("red");
        typeText("Invalid input. Please enter a positive odd number between 3 and 31.", 50);
        changeColor("reset");
        gotoxy(4, 4);
        const std::string prompt = "Enter an odd number for the size of the magic square: ";
        std::cout << prompt;
        gotoxy(4 + prompt.length(), 4);
        std::cout << "                         ";
        gotoxy(4 + prompt.length(), 4);
        std::cin >> n;
        gotoxy(4, 5);
        std::cout << "                                                                    ";
    }
}

void magicScreen() {
    clearScreen();
    changeColor("cyan");
    gotoxy(4, 2);
    std::cout << "=== MAGIC SQUARE ===";
    changeColor("reset");

    int n=0;
    takeValidInput(n);

    gotoxy(4, 6);
    delay(250);
    typeText("Generating Magic Square for input " + std::to_string(n), 50);
    gotoxy(4, 7);
    delay(250);
    typeText("Rows and columns will sum up to " + std::to_string(n * (n * n + 1) / 2), 50);
    gotoxy(4, 8);
    delay(250);
    std::cout << "______________________________________" << std::endl;

    buildMagicSquareBoxes(n, 4, 10);
    populateMagicSquare(n, 4, 10);

    gotoxy(4, 10 + n * 2 + 2);
    changeColor("green");
    typeText("Magic Square of size " + std::to_string(n) + " completed!", 50);
    gotoxy(4, 10 + n * 2 + 3);
    changeColor("reset");

    while (true) {
        Key key = getKeyPress();
        if (key == BACKSPACE) {
            break;
        }
    }
}

void drawMenu(const std::string menuItems[], int menuSize, int selectedIndex) {
    clearScreen();
    changeColor("cyan");
    gotoxy(4, 2);
    std::cout << "=== MAIN MENU ===";
    resetColor();

    for (short i = 0; i < menuSize; i++) {
        gotoxy(4, 4 + i);
        if (i == selectedIndex) {
            changeColor("lightmagenta");
            std::cout << "> " << menuItems[i];
            resetColor();
        } else {
            std::cout << "  " << menuItems[i];
        }
    }
}

void handleMenuInput(const std::string menuItems[], int menuSize, int &selectedIndex) {
    Key key = getKeyPress();
    switch (key) {
        case UP:
        case LEFT:
            if (selectedIndex == 0) selectedIndex = menuSize - 1;
            else selectedIndex--;
            break;
        case DOWN:
        case RIGHT:
            if (selectedIndex == menuSize - 1)
                selectedIndex = 0;
            else selectedIndex++;
            break;
        case HOME:
            selectedIndex = 0;
            break;
        case END:
            selectedIndex = menuSize - 1;
            break;
        case ENTER:
            if (menuItems[selectedIndex] == "Magic Square") {
                magicScreen();
            } else if (menuItems[selectedIndex] == "Exit") {
                clearScreen();
                exit(0);
            }
            break;
        case ESCAPE:
            clearScreen();
            exit(0);
        default:
            break;
    }
}

int main() {
    std::string menuItems[] = { "Magic Square", "Exit" };
    int menuSize = sizeof(menuItems) / sizeof(menuItems[0]);
    int selectedIndex = 0;

     while (true) {
        drawMenu(menuItems, menuSize, selectedIndex);
        handleMenuInput(menuItems, menuSize, selectedIndex);
    }

    return 0;
}
