#ifndef HELPERS_H
#define HELPERS_H

#include <string>

struct DOB {
    int day;
    int month;
    int year;
};

struct Employee {
    int salary;
    DOB dob;
    std::string firstName;
    std::string lastName;
};

bool isInt(const std::string &input);
bool isAlpha(const std::string &input);
bool isValidDOB(const std::string &input);
DOB extractDOB(std::string input);

#endif