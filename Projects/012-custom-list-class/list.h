#ifndef LIST_H
#define LIST_H

#include "listitem.h"
#include <string>
#include <iostream>

class List {
    private:
        ListItem *ptr;
        int filledSize;
        int totalCapacity;
        void growIfNeeded();

    public:
        List(int size = 10);
        List(const List &other);
        List(std::initializer_list<ListItem> items);
        ~List();
        List& operator=(const List &other);

        int getSize() const;
        void setSize(int size);

        void add(int value);
        void add(char value);
        void add(bool value);
        void add(float value);
        void add(double value);
        void add(std::string value);
        void add(const char* value);
        void add(const ListItem& value);

        void remove();
        void clear();
        void fullPrint();
        void detailPrint();
        ListItem& operator[](int idx);
};

#endif