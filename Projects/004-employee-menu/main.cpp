#include <iostream>
#include "library.h"
#include "employees.h"
#define MAX_EMPLOYEES 100

void newScreen(Employee employees[], int &employeeCount) {
    clearScreen();
    changeColor("cyan");
    gotoxy(4, 2);
    std::cout << "=== Register a New Employee ===";
    resetColor();

    Employee e;

    gotoxy(4, 4);
    std::cout << "";

    Key key = getKeyPress();
    if (key == ESCAPE) {
        return;
    }

    getValidFirstName(e);
    getValidLastName(e);
    getValidSalary(e);
    getValidDOB(e);

    employees[employeeCount] = e;
    employeeCount++;

    gotoxy(4, 12);
    changeColor("lightgreen");
    std::cout << "Employee registered successfully!";

    gotoxy(4, 13);
    changeColor("yellow");
    std::cout << "TAB > add another Employee | BACKSPACE > Main Menu.";
    resetColor();

    while (true) {
        Key key = getKeyPress();
        if (key == BACKSPACE) return;
        if (key == TAB) { 
            newScreen(employees, employeeCount);
            return;
        }
    }
}

void displayScreen(Employee employees[], int employeeCount) {
    clearScreen();
    changeColor("cyan");
    gotoxy(4, 2);
    std::cout << "=== Registered Employees ===";
    resetColor();
    gotoxy(4, 4);
    for (int i = 0; i < employeeCount; i++) {
        gotoxy(4, 4 + i);
        std::cout << i + 1 << ". " << employees[i].firstName;
        gotoxy(4 + 10, 4 + i);
        std::cout << employees[i].lastName;
        gotoxy(4 + 20, 4 + i);
        std::cout << employees[i].salary;
        gotoxy(4 + 28, 4 + i);
        std::cout << employees[i].dob.day << "/" << employees[i].dob.month << "/" << employees[i].dob.year;
    }
    while (true)
    {
        Key key = getKeyPress();
        if (key == BACKSPACE) {
            return;
        } else if (key == ESCAPE) {
            clearScreen();
            exit(0);
        }
    }
}

void getMenuInput(int &selectedIndex, int menuSize, int &employeeCount, Employee employees[]) {
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
                if (selectedIndex == 0) {
                    newScreen(employees, employeeCount);
                } else if (selectedIndex == 1) {
                    displayScreen(employees, employeeCount);
                } else {
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

int main() {
    Employee employees[MAX_EMPLOYEES];
    int employeeCount = 0;

    std::string menuItems[] = { "New Employee", "Display Employees", "Exit" };
    int menuSize = sizeof(menuItems) / sizeof(menuItems[0]);
    int selectedIndex = 0;

    while (true) {
        drawMenu(menuItems, menuSize, selectedIndex);
        getMenuInput(selectedIndex, menuSize, employeeCount, employees);
     }

    return 0;
}
