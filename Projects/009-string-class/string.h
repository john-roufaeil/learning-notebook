#ifndef STRING_H
#define STRING_H

#include<iostream>

class String {
    private:
        int size;
        char* str;

    public:
        String(int size);
        String(const char* inputString = "");
        String(const String &other);
        ~String();
        void setSize(int n);
        int getSize();
        void setStr(char *);
        char *getStr();
        void fullPrint();
        String &operator=(const String &other);
        char& operator[](int idx); // modifiable
        const char& operator[](int idx) const; //readonly
};

std::ostream &operator<<(std::ostream &os, String &s);
std::istream &operator>>(std::istream &is, String &s);

#endif