#ifndef STRING_H
#define STRING_H

#include<iostream>

class String {
    private:
        int size;
        char* str;

    public:
        String(const char* str = "");
        String(int n);
        ~String();
        int getSize();
        void setSize(int n);
        char *getStr();
        void setStr(char *);
        void fullPrint();
        char& operator[](int idx);
        // copy
};

std::ostream &operator<<(std::ostream &os, String &s);
// std::istream &operator>>(std::istream &is, String &s);


#endif