#include <iostream>
#include "employees.h"

void getValidFirstName(Employee &e) {
    gotoxy(4, 4);
    std::string firstNamePrompt = "First Name: ";
    std::cout << firstNamePrompt;
    while (true) {
        std::getline(std::cin, e.firstName);
        
        if (!e.firstName.empty() && isAlpha(e.firstName)) {
            gotoxy(4, 5);
            std::cout << "                                                   ";
            return;
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
            return;
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
            return;
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
            return;
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
