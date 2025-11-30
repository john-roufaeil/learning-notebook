#include "string.h"
#include <stdexcept>
#include <iostream>

// Constructors & Destructor --------------------

String::String(int size) {
    if (size < 1) throw std::invalid_argument("Size must cannot be less than 1");
    this->size = size;
    this->str = new char[size]();
    this->str[size - 1] = '\0';
}

String::String(const char* inputString) {
    int len = 0;
    while (inputString[len] != '\0') {
        len++;
    }
    this->size = len + 1;

    this->str = new char[this->size]();
    for (int i = 0; i < len; i++) {
        this->str[i] = inputString[i];
    }
    this->str[len] = '\0';
}

String::String(const String& other) {
    int len = 0;
    while (other[len] != '\0') {
        len++;
    }
    this->size = len + 1;

    this->str = new char[this->size]();
    for (int i = 0; i < len; i++) {
        this->str[i] = other[i];
    }
    this->str[len] = '\0';
}

String::~String() {
    delete[] this->str;
}

// Setters & Getters ----------------------------

void String::setSize(int newSize) {
    if (newSize < 1) throw std::invalid_argument("Size cannot be less than 1");

    char *newStr = new char[newSize]();
    int limit = (newSize < this->size) ? newSize - 1: this->size - 1;
    for (int i = 0; i < limit; i++) {
        newStr[i] = this->str[i];
    }
    newStr[limit] = '\0';
    delete[] str;
    this->str = newStr;
    this->size = newSize;
}

int String::getSize() const {
    return this->size;
}

void String::setStr(const char* newStr) {
    if (!newStr) return;
    int newLen = 0;
    while (newStr[newLen] != '\0') newLen++;
    if (newLen + 1 > this->size) {
        delete[] str;
        this->size = newLen + 1;
        this->str = new char[this->size]();
    }

    for (int i = 0; i < newLen; i++) {
        this->str[i] = newStr[i];
    }
    this->str[newLen] = '\0';
}

const char* String::getStr() const {
    return this->str;
}

// Core Fns -------------------------------------

int String::length() const {
    int idx = 0;
    while (this->str[idx] != '\0') {
        idx++;
    }
    return idx;
}

bool String::isEmpty() const {
    return this->str[0] == '\0';
}

void String::resize(int newSize) {
    char *newStr = new char[newSize]();
    int limit = (newSize < this->size) ? newSize - 1: this->size - 1;
    for (int i = 0; i < limit; i++) {
        newStr[i] = this->str[i];
    }
    newStr[limit] = '\0';
    delete[] this->str;
    this->str = newStr;
    this->size = newSize;
}

void String::clear() {
    this->str[0] = '\0';
}

String& String::operator=(const char *inputString) { 
    int newLen = 0;
    while(inputString[newLen] != '\0') {
        newLen++;
    }
    newLen++;
    
    this->setStr(inputString);
    this->setSize(newLen);
    return *this;
}

String& String::operator=(const String& other) {
    if (this == &other) {
        return *this;
    }

    int newLen = other.getSize();
    this->setStr(other.getStr());
    this->setSize(newLen);
    return *this;
}

char& String::operator[](int idx) {
    if (idx >= this-> size) throw std::invalid_argument("Index out of bound.");
    return this->str[idx];
}

const char& String::operator[](int idx) const {
    if (idx >= this-> size) throw std::invalid_argument("Index out of bound.");
    return this->str[idx];
}

void String::fullPrint() const {
    std::cout << "Size: " << this->getSize() << " chars, Data: " << this->getStr() << std::endl;
}

std::ostream& operator<<(std::ostream& os, const String& s) {
    os << s.getStr();
    return os;
}
