#include "string.h"
#include <stdexcept>
#include <iostream>

String::String(int size) {
    if (size < 1) throw std::invalid_argument("Size cannot be less than 1");
    this->size = size;
    str = new char[size]();
}

String::String(const char* inputString) {
    int len = 0;
    while (inputString[len] != '\0') {
        len++;
    }
    size = len + 1;

    str = new char[size];
    for (int i = 0; i < size; i++) {
        str[i] = inputString[i];
    }
}

String::String(const String& other) {
    size = other.size;
    str = new char[size];
    for (int i = 0; i < other.size; i++) {
        str[i] = other.str[i];
    }
}

String::String(String&& other) {
    str = other.str;
    size = other.size;
    other.str = nullptr;
    other.size = 0;
}

String& String::operator=(const char *inputString) { 
    if (str == inputString) return *this;
    int len = 0;
    while(inputString[len] != '\0') {
        len++;
    }
    len++;

    if (len != size) {
        delete[] str;
        size = len;
        str = new char[size];
    }
    for (int i = 0; i < size; i++) {
        str[i] = inputString[i];
    }
    return *this;
}

String& String::operator=(const String& other) {
    if (this == &other) return *this;
    if (size != other.size) {
        delete[] str;
        size = other.size;
        str = new char[size];
    }
    for (int i = 0; i < size; i++) {
        str[i] = other[i];
    }
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

void String::setSize(int newSize) {
    if (newSize < 1) throw std::invalid_argument("Size cannot be less than 1");
    if (size == newSize) return;

    char *newStr = new char[newSize]();
    int limit = (newSize < size) ? newSize - 1: size - 1;
    for (int i = 0; i < limit; i++) {
        newStr[i] = str[i];
    }
    newStr[newSize - 1] = '\0';
    delete[] str;
    str = newStr;
    size = newSize;
}

int String::getSize() const {
    return size;
}

void String::setStr(const char* newStr) {
    if (!newStr) return;
    int newLen = 0;
    while (newStr[newLen] != '\0') newLen++;
    if (newLen + 1 > size) {
        delete[] str;
        size = newLen + 1;
        str = new char[size]();
    }

    for (int i = 0; i < newLen; i++) {
        str[i] = newStr[i];
    }
    str[newLen] = '\0';
}

const char* String::getStr() const {
    return str;
}

int String::length() const {
    int idx = 0;
    while (str[idx] != '\0') {
        idx++;
    }
    return idx;
}

bool String::isEmpty() const {
    return str[0] == '\0';
}

void String::clear() {
    str[0] = '\0';
}

void String::append(const String& s) {
    int oldLen = length();
    int addLen = s.length();

    setSize(oldLen + addLen + 1);

    for (int i = 0; i < addLen; i++)
        str[oldLen + i] = s[i];

    str[oldLen + addLen] = '\0';
}

void String::append(const char* s) {
    int oldLen = length();

    int addLen = 0;
    while (s[addLen] != '\0') addLen++;

    setSize(oldLen + addLen + 1);

    for (int i = 0; i < addLen; i++)
        str[oldLen + i] = s[i];

    str[oldLen + addLen] = '\0';
}

void String::append(const char c) {
    int oldLen = length();

    setSize(oldLen + 2);

    str[oldLen] = c;
    str[oldLen + 1] = '\0';
}

String String::operator+(const String& other) const {
    String result(length() + other.length() + 1);

    for (int i = 0; i < length(); i++) {
        result[i] = str[i];
    }
    for (int i = 0; i < other.length(); i++) {
        result[i + length()] = other[i];
    }

    result[length() + other.length()] = '\0';
    return result;
}

String String::operator+(const char* other) const {
    int otherLen = 0;
    while (other[otherLen] != '\0') otherLen++;

    String result(length() + otherLen + 1);

    for (int i = 0; i < length(); i++){
        result[i] = str[i];
    }
    for (int i = 0; i < otherLen; i++) {
        result[i + length()] = other[i];
    }

    result[length() + otherLen] = '\0';
    return result;
}

String String::operator+(const char c) const {
    String result(length() + 2);

    for (int i = 0; i < length(); i++) {
        result[i] = str[i];
    }

    result[length()] = c;
    result[length() + 1] = '\0';
    return result;
}

String& String::operator+=(const String& s) {
    append(s);
    return *this;
}

String& String::operator+=(const char* s) {
    append(s);
    return *this;
}

String& String::operator+=(char c) {
    append(c);
    return *this;
}

bool String::operator==(const String& other) const {
    int i = 0;
    while (str[i] != '\0' && other[i] != '\0') {
        if (str[i] != other[i]) {
            return false;
        }
        i++;
    }
    return str[i] == '\0' && other[i] == '\0';
}

bool String::operator==(const char* other) const {
    int i = 0;
    while (str[i] != '\0' && other[i] != '\0') {
        if (str[i] != other[i]) {
            return false;
        }
        i++;
    }
    return str[i] == '\0' && other[i] == '\0';
}

bool String::operator!=(const String& other) const {
    return !(*this == other);
}

bool String::operator!=(const char* other) const {
    return !(*this == other);
}

bool String::operator<(const String& other) const {
    int i = 0;
    while (str[i] != '\0' && other[i] != '\0') {
        if (str[i] < other[i]) return true;
        if (str[i] > other[i]) return false;
        i++;
    }
    return length() < other.length();
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
    if (length() > 1 && str[0] == '-') {
        isNegative = true;
        start = 1;
    }
    for (int i = start; i < length(); i++) {
        if ((str[i] > '9' || str[i] < '0')) {
            throw std::invalid_argument("String is not an integer");
        }
        value = value * 10 + (str[i] - '0');
    }

    return isNegative ? -value : value;
}

void String::toLower() {
    for (int i = 0; i < length(); i++) {
        if (str[i] >= 'A' && str[i] <= 'Z')
            str[i] = str[i] - 'A' + 'a';
    }
}

void String::toUpper() {
    for (int i = 0; i < length(); i++) {
        if (str[i] >= 'a' && str[i] <= 'z')
            str[i] = str[i] - 'a' + 'A';
    }
}

char& String::operator[](int idx) {
    if (idx < 0 || idx >= size) throw std::invalid_argument("Index out of bound.");
    return str[idx];
}

const char& String::operator[](int idx) const {
    if (idx < 0 || idx >= size) throw std::invalid_argument("Index out of bound.");
    return str[idx];
}

void String::fullPrint() const {
    std::cout << "Size: " << getSize() << " chars, Data: " << getStr() << std::endl;
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
