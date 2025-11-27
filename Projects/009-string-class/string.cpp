#include "string.h"
#include <stdexcept>

void String::setSize(int newSize) {
    if (newSize < 0) {
        throw std::invalid_argument("Size cannot be negative");
    }
    char *newStr = new char[newSize];
    int limit = (newSize < size) ? newSize - 1: this->size - 1;
    for (int i = 0; i < limit; i++) {
        newStr[i] = this->str[i];
    }
    newStr[limit] = '\0';
    delete[] str;
    this->str = newStr;
    this->size = newSize;
}

int String::getSize() {
    return this->size;
}

void String::setStr(char* newStr) {
    if (!newStr) return;
    int newLen = 0;
    while (newStr[newLen] != '\0') newLen++;
    if (newLen + 1 > this->size) {
        delete[] str;
        this->size = newLen + 1;
        this->str = new char[this->size];
    }

    for (int i = 0; i < newLen; i++) {
        this->str[i] = newStr[i];
    }
    this->str[newLen] = '\0';
}

char* String::getStr() {
    return this->str;
}

String::String(int size) {
    this->str = new char[size];
    this->str[0] = '\0';
    this->size = size;
}

String::String(const char* inStr) {
    int len = 0;
    while (inStr[len] != '\0')
        len++;
    this->size = len + 1;
    this->str = new char[this->size];
    for (int i = 0; i < len; i++) {
        this->str[i] = inStr[i];
    }
    this->str[len] = '\0';
}

String::~String() {
    delete[] this->str;
}
