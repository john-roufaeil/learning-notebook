#ifndef EMPLOYEES_H
#define EMPLOYEES_H

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


int getValidFirstName(Employee &e);
int getValidLastName(Employee &e);
int getValidSalary(Employee &e);
int getValidDOB(Employee &e);

#endif