#include <iostream>
#include "terminal.h"
#include "helpers.h"
#include "employees.h"

int getValidFirstName(Employee &e) {
    gotoxy(4, 4);
    std::string firstNamePrompt = "First Name: ";
    std::cout << firstNamePrompt;
    while (true) {
        std::getline(std::cin, e.firstName);
        if (e.firstName.empty()) return 1;
        
        gotoxy(4, 5);
        std::cout << "                                                   ";
        if (isAlpha(e.firstName)) return 0;
        
        gotoxy(4 + firstNamePrompt.length(), 4);
        std::cout << "           ";
        gotoxy(4,5);
        changeColor("red");
        std:: cout<< "First name must contain only alphabetic characters.";
        resetColor();
        gotoxy(4 + firstNamePrompt.length(), 4);
    }
}

int getValidLastName(Employee &e) {
    gotoxy(4, 6);
    std::string lastNamePrompt = "Last Name: ";
    std::cout << lastNamePrompt;
    while (true) {
        std::getline(std::cin, e.lastName);
        if (e.lastName.empty()) return 1;

        gotoxy(4, 7);
        std::cout << "                                                   ";
        if (isAlpha(e.lastName)) return 0;
        
        gotoxy(4 + lastNamePrompt.length(), 6);
        std::cout << "           ";
        gotoxy(4, 7);
        changeColor("red");
        std:: cout<< "Last name must contain only alphabetic characters.";
        resetColor();
        gotoxy(4 + lastNamePrompt.length(), 6);
    }
}

int getValidSalary(Employee &e) {
    gotoxy(4, 8);
    std::string salaryPrompt = "Salary: ";
    std::cout << salaryPrompt;
    while (true) {
        std::string salaryInput;
        std::getline(std::cin, salaryInput);

        gotoxy(4, 9);
        std::cout << "                                                                             ";
        if (salaryInput.empty()) return 1;
        
        if (isInt(salaryInput) && std::stoi(salaryInput) >= 1000 && std::stoi(salaryInput) <= 1000000) {
            e.salary = std::stoi(salaryInput);
            return 0;
        }

        gotoxy(4 + salaryPrompt.length(), 8);
        std::cout << "           ";
        changeColor("red");
        gotoxy(4, 9);
        if (!isInt(salaryInput)) std::cout << "Salary must be a valid integer.";
        else if (e.salary < 1000 || e.salary > 1000000) std:: cout<< "Salary must be between 1,000 and 1,000,000.";
        resetColor();
        gotoxy(4 + salaryPrompt.length(), 8);
    }
}

int getValidDOB(Employee &e) {
    gotoxy(4, 10);
    std::string dobPrompt = "Date of Birth (DD MM YYYY): ";
    std::cout << dobPrompt;
    while (true) {
        std::string dobInput;
        std::getline(std::cin, dobInput);
        
        gotoxy(4, 11);
        std::cout << "                                                                             ";
        if (dobInput.empty()) return 1;
        if (isValidDOB(dobInput)) {
            DOB dob = extractDOB(dobInput);
            e.dob = dob;
            return 0;
        }
        gotoxy(4 + dobPrompt.length(), 10);
        std::cout << "                     ";
        changeColor("red");
        gotoxy(4, 11);
        std::cout << "Enter numbers only (DD MM YYYY) between 01/01/1920 and 31/12/2005";
        resetColor();
        gotoxy(4 + dobPrompt.length(), 10);
    }
}
