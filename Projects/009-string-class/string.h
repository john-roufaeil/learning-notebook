#ifndef STRING_H
#define STRING_H

#include<iostream>

class String {
    private:
        int size; // including null terminator
        char* str;

    public:
        String(int size);
        String(const char* inputString = "");
        String(const String &other);
        ~String();

        void setSize(int n);
        int getSize() const;
        void setStr(const char * inputString);
        const char *getStr() const;

        int length() const;
        bool isEmpty() const;
        void resize(int newSize);
        void clear();
        String& operator=(const char *inputString);
        String& operator=(const String &other);
        char& operator[](int idx); // modifiable
        const char& operator[](int idx) const; //readonly
        
        void append(const String& StringToAppend);
        void append(const char *StringToAppend);
        void append(const char charToAppend);
        String operator+(const String &StringToConcat) const;
        String operator+(const char *StringToConcat) const;
        String operator+(const char charToConcat) const;
        String& operator+=(const String &StringToConcat);
        String& operator+=(const char *StringToConcat);
        String& operator+=(const char charToConcat);

        bool operator==(const String &other) const;
        bool operator==(const char *other) const;
        bool operator!=(const String &other) const;
        bool operator!=(const char *other) const;

        bool operator>(const String &other) const;
        bool operator<(const String &other) const;
        bool operator>=(const String &other) const;
        bool operator<=(const String &other) const;

        int toInt() const;
        void toLower();
        void toUpper();
        void fullPrint() const;
};

std::ostream &operator<<(std::ostream &os, const String &s);
std::istream &operator>>(std::istream &ios, String &s);

#endif