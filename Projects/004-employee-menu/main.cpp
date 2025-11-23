#include <iostream>
#include "library.h"

#define MAX_EMPLOYEES 100

void getValidFirstName(Employee &e) {
    gotoxy(4, 4);
    std::string firstNamePrompt = "First Name: ";
    std::cout << firstNamePrompt;
    while (true) {
        std::getline(std::cin, e.firstName);
        
        if (!e.firstName.empty() && isAlpha(e.firstName)) {
            gotoxy(4, 5);
            std::cout << "                                                   ";
            break;
        }
        
        gotoxy(4 + firstNamePrompt.length(), 4);
        std::cout << "           ";
        gotoxy(4,5);
        changeColor("red");
        if (e.firstName.empty()) {
            std::cout << "                                                                             ";
            gotoxy(4,5);
            std::cout << "First name cannot be empty.";
        } else if (!isAlpha(e.firstName)) {
            std::cout << "                                                                             ";
            gotoxy(4,5);
            std:: cout<< "First name must contain only alphabetic characters.";
        }
        resetColor();
        gotoxy(4 + firstNamePrompt.length(), 4);
    }
}

void getValidLastName(Employee &e) {
    gotoxy(4, 6);
    std::string lastNamePrompt = "Last Name: ";
    std::cout << lastNamePrompt;
    while (true) {
        std::getline(std::cin, e.lastName);

        if (!e.lastName.empty() && isAlpha(e.lastName)) {
            gotoxy(4, 7);
            std::cout << "                                                   ";
            break;
        }
        
        gotoxy(4 + lastNamePrompt.length(), 6);
        std::cout << "           ";
        gotoxy(4, 7);
        changeColor("red");
        if (e.lastName.empty()) {
            std::cout << "                                                                             ";
            gotoxy(4,7);
            std::cout << "Last name cannot be empty.";
        } else if (!isAlpha(e.lastName)) {
            std::cout << "                                                                             ";
            gotoxy(4,7);
            std:: cout<< "Last name must contain only alphabetic characters.";
        }
        resetColor();
        gotoxy(4 + lastNamePrompt.length(), 6);
    }
}

void getValidSalary(Employee &e) {
    gotoxy(4, 8);
    std::string salaryPrompt = "Salary: ";
    std::cout << salaryPrompt;
    while (true) {
        std::string salaryInput;
        std::getline(std::cin, salaryInput);

        if (isInt(salaryInput) && std::stoi(salaryInput) >= 1000 && std::stoi(salaryInput) <= 1000000) {
            gotoxy(4, 9);
            std::cout << "                                                                             ";
            e.salary = std::stoi(salaryInput);
            break;
        }

        gotoxy(4 + salaryPrompt.length(), 8);
        std::cout << "           ";
        gotoxy(4, 9);
        changeColor("red");
        if (!isInt(salaryInput)) {
            std::cout << "                                                                             ";
            gotoxy(4, 9);
            std::cout << "Salary must be a valid integer.";
        } else if (e.salary < 1000 || e.salary > 1000000) {
            std::cout << "                                                                             ";
            gotoxy(4, 9);
            std:: cout<< "Salary must be between 1,000 and 1,000,000.";
        }
        resetColor();
        gotoxy(4 + salaryPrompt.length(), 8);
    }
}

void getValidDOB(Employee &e) {
    gotoxy(4, 10);
    std::string dobPrompt = "Date of Birth (DD MM YYYY): ";
    std::cout << dobPrompt;
    while (true) {
        std::string dobInput;
        std::getline(std::cin, dobInput);

        if (isValidDOB(dobInput)) {
            DOB dob = extractDOB(dobInput);
            e.dob = dob;
            gotoxy(4, 11);
            std::cout << "                                                                             ";
            break;
        }

        gotoxy(4 + dobPrompt.length(), 10);
        std::cout << "                     ";
        gotoxy(4, 11);
        changeColor("red");
        std::cout << "                                                      ";
        gotoxy(4, 11);
        std::cout << "Enter numbers only (DD MM YYYY) between 01/01/1920 and 31/12/2024";

        resetColor();
        gotoxy(4 + dobPrompt.length(), 10);
    }
}

void newScreen(Employee employees[], int &employeeCount) {
    clearScreen();
    changeColor("cyan");
    gotoxy(4, 2);
    std::cout << "=== Register a New Employee ===";
    resetColor();

    Employee e;

    getValidFirstName(e);
    getValidLastName(e);
    getValidSalary(e);
    getValidDOB(e);

    employees[employeeCount++] = e;

    gotoxy(4, 12);
    changeColor("lightgreen");
    std::cout << "Employee registered successfully!";

    gotoxy(4, 13);
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
        employees[i].salary << " " << employees[i].dob.day << "/" <<
        employees[i].dob.month << "/" << employees[i].dob.year;
    }
    while (true)
    {
        Key key = getKeyPress();
        if (key == BACKSPACE) {
            break;
        }
        else if (key == ESCAPE) {
            clearScreen();
            exit(0);
        }
    }
}

void getMenuInput(const std::string menuItems[], int &selectedIndex, int menuSize, int employeeCount, Employee employees[]) {
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

    std::string menuItems[] = { "New", "Display", "Exit" };
    int menuSize = sizeof(menuItems) / sizeof(menuItems[0]);
    int selectedIndex = 0;

    while (true) {
        drawMenu(menuItems, menuSize, selectedIndex);
        getMenuInput(menuItems, selectedIndex, menuSize, employeeCount, employees);
    }

    return 0;
}
