#include <iostream>
#include "terminal.h"
#include "helpers.h"
#include "employees.h"
#define MAX_EMPLOYEES 10

void newScreen(Employee employees[], int &employeeCount) {
    clearScreen();
    gotoxy(4, 2);
    if (employeeCount < MAX_EMPLOYEES) {
        changeColor("cyan");
        std::cout << "=== Register a New Employee ===";
        resetColor();

        Employee e;
        if (getValidFirstName(e) == 1) return;
        if (getValidLastName(e) == 1) return;
        if (getValidSalary(e) == 1) return;
        if (getValidDOB(e) == 1) return;

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
    } else {
        changeColor("red");
        std::cout << "=== Maximum Amount of Employees Registered ===";

        gotoxy(4, 4);
        changeColor("yellow");
        std::cout << "BACKSPACE > Main Menu.";
        resetColor();

        while (true) {
            Key key = getKeyPress();
            if (key == BACKSPACE) return;
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
        std::cout << i + 1 << ". " << employees[i].firstName << " " 
        << employees[i].lastName << " " <<  employees[i].salary << " "
        << employees[i].dob.day << "/" << employees[i].dob.month << "/"
        << employees[i].dob.year;
    }
    while (true) {
        Key key = getKeyPress();
        if (key == BACKSPACE) {
            return;
        } else if (key == ESCAPE) {
            clearScreen();
            exit(0);
        }
    }
}

void getMenuInput(
    int &selectedIndex, int menuSize, 
    int &employeeCount, Employee employees[]
) {
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

int main() {
    Employee employees[MAX_EMPLOYEES];
    int employeeCount = 0;

    std::string menuItems[] = { "New Employee", "Display Employees", "Exit" };
    int menuSize = sizeof(menuItems) / sizeof(menuItems[0]);
    int selectedIndex = 0;

    while (true) {
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
        getMenuInput(selectedIndex, menuSize, employeeCount, employees);
     }

    return 0;
}
