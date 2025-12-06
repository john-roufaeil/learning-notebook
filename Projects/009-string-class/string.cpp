#include "string.h"
#include <stdexcept>
#include <iostream>

// Constructors & Destructor --------------------

String::String(int size) {
    if (size < 1) throw std::invalid_argument("Size must cannot be less than 1");
    this->size = size;
    str = new char[size];
    str[size - 1] = '\0';
}

String::String(const char* inputString) {
    int len = 0;
    while (inputString[len] != '\0') {
        len++;
    }
    size = len + 1;

    str = new char[size];
    for (int i = 0; i < len; i++) {
        str[i] = inputString[i];
    }
    str[len] = '\0';
}

String::String(const String& other) {
    size = other.size;

    str = new char[size];
    for (int i = 0; i < other.length(); i++) {
        str[i] = other[i];
    }
    str[other.length()] = '\0';
}

String::String(String&& other) {
    str = other.str;
    size = other.size;
    other.str = nullptr;
    other.size = 0;
}

String& String::operator=(const char *inputString) { 
    int newLen = 0;
    while(inputString[newLen] != '\0') {
        newLen++;
    }
    newLen++;

    if (newLen != size) {
        delete[] str;
        size = newLen;
        str = new char[size];
    }
    for (int i = 0; i < size - 1; i++) {
        str[i] = inputString[i];
    }
    str[size - 1] = '\0';
    return *this;
}

String& String::operator=(const String& other) {
    if (this == &other) return *this;
    delete[] str;
    size = other.size;
    str = new char[size];
    for (int i = 0; i < size - 1; i++) {
        str[i] = other[i];
    }
    str[size - 1] = '\0'; 
    return *this;
}

String& String::operator=(String && other) {
    if (this == &other) return *this;

    delete[] str;
    str = other.str;
    size = other.size;
    other.str = nullptr;
    other.size = 0;
    return *this;
}

String::~String() {
    delete[] str;
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

void String::append(const String& s) {
    int oldLen = this->length();
    int addLen = s.length();

    this->resize(oldLen + addLen + 1);

    for (int i = 0; i < addLen; i++)
        this->str[oldLen + i] = s[i];

    this->str[oldLen + addLen] = '\0';
}

void String::append(const char* s) {
    int oldLen = this->length();

    int addLen = 0;
    while (s[addLen] != '\0') addLen++;

    this->resize(oldLen + addLen + 1);

    for (int i = 0; i < addLen; i++)
        this->str[oldLen + i] = s[i];

    this->str[oldLen + addLen] = '\0';
}

void String::append(const char c) {
    int oldLen = this->length();

    this->resize(oldLen + 2);

    this->str[oldLen] = c;
    this->str[oldLen + 1] = '\0';
}

String String::operator+(const String& other) const {
    String result(this->length() + other.length() + 1);

    for (int i = 0; i < this->length(); i++) {
        result[i] = this->str[i];
    }
    for (int i = 0; i < other.length(); i++) {
        result[i + this->length()] = other[i];
    }

    result[this->length() + other.length()] = '\0';
    return result;
}

String String::operator+(const char* other) const {
    int otherLen = 0;
    while (other[otherLen] != '\0') otherLen++;

    String result(this->length() + otherLen + 1);

    for (int i = 0; i < this->length(); i++){
        result[i] = this->str[i];
    }
    for (int i = 0; i < otherLen; i++) {
        result[i + this->length()] = other[i];
    }

    result[this->length() + otherLen + 1] = '\0';
    return result;
}

String String::operator+(const char c) const {
    String result(this->length() + 2);

    for (int i = 0; i < this->length(); i++) {
        result[i] = this->str[i];
    }

    result[this->length()] = c;
    result[this->length() + 1] = '\0';
    return result;
}

String& String::operator+=(const String& s) {
    this->append(s);
    return *this;
}

String& String::operator+=(const char* s) {
    this->append(s);
    return *this;
}


String& String::operator+=(char c) {
    this->append(c);
    return *this;
}

bool String::operator==(const String& other) const {
    int i = 0;
    while (this->str[i] != '\0' && other[i] != '\0') {
        if (this->str[i] != other[i]) {
            return false;
        }
        i++;
    }
    return true;
}

bool String::operator==(const char* other) const {
    int i = 0;
    while (this->str[i] != '\0' && other[i] != '\0') {
        if (this->str[i] != other[i]) {
            return false;
        }
        i++;
    }
    return true;
}

bool String::operator!=(const String& other) const {
    return !(*this == other);
}

bool String::operator!=(const char* other) const {
    return !(*this == other);
}

bool String::operator<(const String& other) const {
    int i = 0;
    while (this->str[i] != '\0' && other[i] != '\0') {
        if (this->str[i] < other[i]) return true;
        if (this->str[i] > other[i]) return false;
        i++;
    }
    return this->length() < other.length();
}

bool String::operator>(const String& other) const {
    return other < *this;
}

bool String::operator<=(const String& other) const {
    return !(*this > other);
}

bool String::operator>=(const String& other) const {
    return !(*this < other);
}

int String::toInt() const {
    int value = 0;
    bool isNegative = false;
    int start = 0;
    if (this->length() > 1 && this->str[0] == '-') {
        isNegative = true;
        start = 1;
    }
    for (int i = start; i < this->length(); i++) {
        if ((this->str[i] > '9' || this->str[i] < '0')) {
            throw std::invalid_argument("String is not an integer");
        }
        value = value * 10 + (this->str[i] - '0');
    }

    return isNegative ? -value : value;
}

void String::toLower() {
    for (int i = 0; i < this->length(); i++) {
        if (str[i] >= 'A' && str[i] <= 'Z')
            str[i] = str[i] - 'A' + 'a';
    }
}

void String::toUpper() {
    for (int i = 0; i < this->length(); i++) {
        if (str[i] >= 'a' && str[i] <= 'z')
            str[i] = str[i] - 'a' + 'A';
    }
}

char& String::operator[](int idx) {
    if (idx < 0 || idx >= length()) throw std::invalid_argument("Index out of bound.");
    return str[idx];
}

const char& String::operator[](int idx) const {
    if (idx < 0 || idx >= length()) throw std::invalid_argument("Index out of bound.");
    return str[idx];
}

void String::fullPrint() const {
    std::cout << "Size: " << this->getSize() << " chars, Data: " << this->getStr() << std::endl;
}

std::ostream& operator<<(std::ostream& os, const String& s) {
    os << s.getStr();
    return os;
}

std::istream& operator>>(std::istream& is, String& s) {
    std::cout << "Input string: ";
    char buffer[1024];
    is >> buffer;
    s = buffer;
    return is;
}