#include "library.h"
#include <iostream>

void newScreen(Employee employees[], int& employeeCount) {
    clearScreen();
    changeColor("cyan");
    gotoxy(4, 2);
    std::cout << "=== Register a New Employee ===";
    resetColor();

    Employee e{0,"", "", {0,0,0}};

    // firstname validation
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

    // lastname validation
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

    // salary validation
    gotoxy(4, 8);
    std::string salaryPrompt = "Salary: ";
    std::cout << salaryPrompt;
    while (true) {
        std::string salaryInput;
        std::cin >> salaryInput;

        if (!isInt(salaryInput)) {
            gotoxy(4 + salaryPrompt.length(), 8);
            std::cout << "             ";
            gotoxy(4, 9);
            changeColor("red");
            std::cout << "Salary must be a valid integer.";
            resetColor();
            gotoxy(4 + salaryPrompt.length(), 8);
            continue;
        }

        e.salary = std::stoi(salaryInput);

        if (e.salary < 1000 || e.salary > 1000000) {
            gotoxy(4 + salaryPrompt.length(), 8);
            std::cout << "             ";
            gotoxy(4, 9);
            changeColor("red");
            std::cout << "Salary must be between 1,000 and 1,000,000.";
            resetColor();
            gotoxy(4 + salaryPrompt.length(), 8);
        } else {
            break;
        }
    }
    gotoxy(4, 9);
    std::cout << "                                                      "; 

    // DOB validation
    gotoxy(4, 10);
    std::string dobPrompt = "Date of Birth (DD MM YYYY): ";
    std::cout << dobPrompt;
    while (true) {
        int day, month, year;
        std::cin >> day >> month >> year;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            gotoxy(4 + dobPrompt.length(), 10);
            std::cout << "                     ";
            gotoxy(4, 11);
            changeColor("red");
            std::cout << "Enter numbers only (DD MM YYYY).";
            resetColor();
            gotoxy(4 + dobPrompt.length(), 10);
            continue;
        }

        if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1925 || year > 2025) {
            gotoxy(4 + dobPrompt.length(), 10);
            std::cout << "                     ";
            gotoxy(4, 11);
            changeColor("red");
            std::cout << "Date must be between 01/01/1925 and 01/01/2025.";
            resetColor();
            gotoxy(4 + dobPrompt.length(), 10);
            continue;
        }

        e.dob.day = day;
        e.dob.month = month;
        e.dob.year = year;
        break;
    }
    gotoxy(4, 11);
    std::cout << "                                                      "; 

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
