#ifndef STRING_H
#define STRING_H

class String {
    private:
        int size;
        char* str;

    public:
        String(int s = 10);
        String(const char* str = "");
        ~String();
        int getSize();
        void setSize(int n);
        char *getStr();
        void setStr(char *);
        //copy
};

#endif