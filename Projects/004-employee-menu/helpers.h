#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include "employees.h"

bool isInt(const std::string &input);
bool isAlpha(const std::string &input);
bool isValidDOB(const std::string &input);
DOB extractDOB(std::string input);

#endif