#include "library.h"
#include <iostream>

void buildMagicSquareBoxes(int n, int x, int y) {
    // Draw horizontal lines and top border
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j < n; j++) {
            gotoxy(x + j * 4, y + i * 2);
            std::cout << "+---";
        }
        gotoxy(x + n * 4, y + i * 2);
        std::cout << "+";
    }
    
    // Draw vertical lines
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= n; j++) {
            gotoxy(x + j * 4, y + i * 2 + 1);
            std::cout << "|";
            if (j < n) {
                std::cout << "   ";
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
        delay(500);
        
        // Display the number in the box
        if (i >= 100)         
            gotoxy(x + col * 4 + 1, y + row * 2 + 1);
        else
            gotoxy(x + col * 4 + 2, y + row * 2 + 1);
        std::cout << i;
        std::flush(std::cout);  // Ensure immediate display
        
        if (i % n == 0) {
            // If i is divisible by n, go down
            row = (row + 1) % n;
        } else {
            // If i is not divisible by n, go up and left
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
    displayInstructions("Use backspace to go to main menu", 4, 3);

    int n=0;
    gotoxy(4, 4);
    std::cout << "Enter an odd number for the size of the magic square: ";
    std::cin >> n;
    
    while (n % 2 == 0 || n < 1 || n > 99) {
        gotoxy(4, 5);
        textColor("red");
        std::cout << "Invalid input. Please enter a positive odd number between 1 and 99: ";
        textColor("reset");
        gotoxy(4, 4);
        std::cout << "Enter an odd number for the size of the magic square:               ";
        gotoxy(58, 4);
        std::cin >> n;
        gotoxy(4, 5);
        std::cout << "                                                                    ";
    }

    gotoxy(4, 6);
    delay(1000);
    std::cout << "Generating Magic Square for input " << n << std::endl;
    gotoxy(4, 7);
    delay(1000);
    std::cout << "Rows and columns will sum up to " << n * (n * n + 1) / 2 << std::endl;
    gotoxy(4, 8);
    delay(1000);
    std::cout << "______________________________________" << std::endl;

    buildMagicSquareBoxes(n, 4, 10);
    populateMagicSquare(n, 4, 10);
    gotoxy(4, 10 + n * 2 + 2);
    textColor("green");
    std::cout << "Magic Square of size " << n << " completed!" << std::endl;
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
