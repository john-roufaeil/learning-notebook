#include "library.h"
#include <cstdio>

int main() {
    clearScreen();

    gotoxy(5, 5);
    textColor("red");
    printf("Hello in RED at (5, 5)");

    gotoxy(10, 7);
    textColor("green");
    printf("Hello in GREEN at (10, 7)");

    gotoxy(5, 9);
    textColor("blue");
    printf("Hello in BLUE at (5, 9)");

    gotoxy(10, 11);
    textColor("yellow");
    printf("Hello in YELLOW at (10, 11)");

    gotoxy(5, 13);
    textColor("magenta");
    printf("Hello in MAGENTA at (5, 13)");

    gotoxy(10, 15);
    textColor("cyan");
    printf("Hello in CYAN at (10, 15)");

    gotoxy(0, 20);
    textColor("reset");
    printf("Done!\n");

    return 0;
}
