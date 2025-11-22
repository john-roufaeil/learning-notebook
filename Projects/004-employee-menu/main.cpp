#include "library.h"
#include <iostream>

void newScreen(Employee employees[], int& employeeCount) {
    clearScreen();
    changeColor("cyan");
    gotoxy(4, 2);
    std::cout << "=== Register a New Employee ===";
    resetColor();

    Employee e{0,0,"", "", {0,0,0}};

    gotoxy(4, 4);
    std::string firstNamePrompt = "First Name: ";
    std::cout << firstNamePrompt;
    std::getline(std::cin, e.firstName);
    while (true) {
        if (!e.firstName.empty() && isAlpha(e.firstName)) {
            break;
        }
        
        gotoxy(4 + firstNamePrompt.length(), 4);
        std::cout << "           ";
        gotoxy(4,5);
        changeColor("red");
        if (e.firstName.empty())
            std::cout << "First name cannot be empty.";
        else if (!isAlpha(e.firstName)) {
            std:: cout<< "First name must contain only alphabetic characters.";
        }
        resetColor();
        gotoxy(4 + firstNamePrompt.length(), 4);
        std::getline(std::cin, e.firstName);
    }
    gotoxy(4, 5);
    std::cout << "                                                   ";

    gotoxy(4, 6);
    std::string lastNamePrompt = "Last Name: ";
    std::cout << lastNamePrompt;
    std::getline(std::cin, e.lastName);
    while (true) {
        if (!e.lastName.empty() && isAlpha(e.lastName)) {
            break;
        }
        gotoxy(4 + lastNamePrompt.length(), 6);
        std::cout << "           ";
        gotoxy(4, 7);
        changeColor("red");
        if (e.lastName.empty())
            std::cout << "Last name cannot be empty.";
        else if (!isAlpha(e.lastName)) {
            std:: cout<< "Last name must contain only alphabetic characters.";
        }
        resetColor();
        gotoxy(4 + lastNamePrompt.length(), 6);
        std::getline(std::cin, e.lastName);
    }
    gotoxy(4, 7);
    std::cout << "                                                   ";


    gotoxy(4, 8);
    std::cout << "Age: ";
    std::cin >> e.age;

    gotoxy(4, 10);
    std::cout << "Salary: ";
    std::cin >> e.salary;

    gotoxy(4, 12);
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
