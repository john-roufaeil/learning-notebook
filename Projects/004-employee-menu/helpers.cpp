#include <string>
#include <regex>
#include "helpers.h"

bool isInt(const std::string &input) {
    std::regex intRegex("^-?[0-9]+$");
    return std::regex_match(input, intRegex);
}

bool isAlpha(const std::string &input) {
    std::regex alphaRegex("^[A-Za-z]+$");
    return std::regex_match(input, alphaRegex);
}   

bool isValidDOB(const std::string &input) {
    std::regex dobRegex("^(0[1-9]|[12][0-9]|3[01]) (0[1-9]|1[0-2]) (19[2-9][0-9]|20[01]\\d|202[0-4])$");
    return std::regex_match(input, dobRegex);
}

DOB extractDOB(std::string input) { //DD MM YYYY
    DOB dob{0,0,0};
    input += " ";
    int part = 1; 
    int currNum = 0;
    for (int i = 0; i < input.length(); i++) {
        if  (input[i] != ' ') {
            currNum = currNum * 10 + (input[i] - '0');
        } else {
            if (part == 1) dob.day = currNum;
            else if (part == 2) dob.month = currNum;
            else if (part == 3) dob.year = currNum;
            currNum = 0;
            part++;
        }
    }
    return dob;
}