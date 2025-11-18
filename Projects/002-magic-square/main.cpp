#include "library.h"
#include <iostream>

void buildMagicSquareBoxes(int n, int x, int y) {
    // Draw horizontal lines and top border
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j < n; j++) {
            gotoxy(x + j * 4, y + i * 2);
            typeText("+---", 10);
        }
        gotoxy(x + n * 4, y + i * 2);
        typeText("+", 10);
    }
    
    // Draw vertical lines
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= n; j++) {
            gotoxy(x + j * 4, y + i * 2 + 1);
            typeText("|", 10);
            if (j < n) {
                typeText("   ", 10);
            }
        }
    }
    gotoxy(x, y);
}

void populateMagicSquare(int n, int x, int y) {
    // Start at the middle of the first row
    int row = 0;
    int col = n / 2;
    
    for (int i = 1; i <= n * n; i++) {
        delay(250);
        textColor("random");

        gotoxy(x + col * 4 + (i >= 100 ? 1 : 2), y + row * 2 + 1);
        typeText(std::to_string(i));
        
        if (i % n == 0) {
            row = (row + 1) % n;
        } else {
            row = (row - 1 + n) % n;
            col = (col - 1 + n) % n;
        }
    }
}

void magicScreen() {
    clearScreen();
    textColor("cyan");
    gotoxy(4, 2);
    std::cout << "=== MAGIC SQUARE ===";
    textColor("reset");
    displayInstructions("Builds a magic square based on odd user input.", 4, 2);
    displayInstructions("Use backspace to go to main menu.", 4, 3);

    int n=0;
    gotoxy(4, 4);
    typeText("Enter an odd number for the size of the magic square: ");
    std::cin >> n;
    
    while (n % 2 == 0 || n < 1 || n > 31) {
        gotoxy(4, 5);
        textColor("red");
        typeText("Invalid input. Please enter a positive odd number between 1 and 31.");
        textColor("reset");
        gotoxy(4, 4);
        std::cout << "Enter an odd number for the size of the magic square:               ";
        gotoxy(58, 4);
        std::cin >> n;
        gotoxy(4, 5);
        std::cout << "                                                                    ";
    }

    gotoxy(4, 6);
    delay(500);
    typeText("Generating Magic Square for input " + std::to_string(n));
    gotoxy(4, 7);
    delay(500);
    typeText("Rows and columns will sum up to " + std::to_string(n * (n * n + 1) / 2));
    gotoxy(4, 8);
    delay(500);
    std::cout << "______________________________________" << std::endl;

    buildMagicSquareBoxes(n, 4, 10);
    populateMagicSquare(n, 4, 10);
    gotoxy(4, 10 + n * 2 + 2);
    textColor("green");
    typeText("Magic Square of size " + std::to_string(n) + " completed!");
    gotoxy(4, 10 + n * 2 + 3);
    textColor("reset");

    while (true) {
        Key key = getKeyPress();
        if (key == Key::BACKSPACE) {
            break;
        }
    }
}

int main() {
    Menu mainMenu("Main Menu", 4, 2);
    mainMenu.addItem("Magic Square", magicScreen);
    mainMenu.addItem("Exit", []() {
        clearScreen();
        exit(0);
    });
    mainMenu.run();
    return 0;
}
