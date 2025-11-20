#include "library.h"
#include <iostream>
void newScreen(Employee employees[], int& employeeCount) {
    clearScreen();
    changeColor("cyan");
    gotoxy(4, 2);
    std::cout << "=== Register a New Employee ===";
    resetColor();

    Employee e;

    gotoxy(4, 4);
    std::cout << "First Name: ";
    std::cin >> e.firstName;

    gotoxy(4, 5);
    std::cout << "Last Name: ";
    std::cin >> e.lastName;

    gotoxy(4, 6);
    std::cout << "Age: ";
    std::cin >> e.age;

    gotoxy(4, 7);
    std::cout << "Salary: ";
    std::cin >> e.salary;

    gotoxy(4, 8);
    std::cout << "Date of Birth (DD MM YYYY): ";
    std::cin >> e.dob.day >> e.dob.month >> e.dob.year;

    employees[employeeCount++] = e;

    gotoxy(4, 10);
    changeColor("lightgreen");
    std::cout << "Employee registered successfully!";

    gotoxy(4, 11);
    changeColor("yellow");
    std::cout << "Press TAB to add another employee or BACKSPACE to return to the main menu.";
    resetColor();

    while (true) {
        Key key = getKeyPress();
        if (key == BACKSPACE) break;
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
        std::cout << i + 1 << ". " << employees[i].firstName << " " << employees[i].lastName << " " << 
        employees[i].age << " " << employees[i].salary << " " << employees[i].dob.day << "/" <<
        employees[i].dob.month << "/" << employees[i].dob.year;
    }
    while (true)
    {
        Key key = getKeyPress();
        if (key == BACKSPACE)
        {
            break;
        }
        else if (key == ESCAPE)
        {
            clearScreen();
            exit(0);
        }
    }
}

int main() {
    Employee employees[100];
    int employeeCount = 0;

    std::string menuItems[] = { "New", "Display", "Exit" };
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
                if (menuItems[selectedIndex] == "New") {
                    newScreen(employees, employeeCount);
                } else if (menuItems[selectedIndex] == "Display") {
                    displayScreen(employees, employeeCount);
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

    return 0;
}
