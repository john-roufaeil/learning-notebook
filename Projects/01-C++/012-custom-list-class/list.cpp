#include "listitem.h"
#include "list.h"
#include <iostream>

List::List(int size) {
    totalCapacity = size;
    filledSize = 0;
    ptr = new ListItem[size]();
}

List::List(const List& other) {
    totalCapacity = other.totalCapacity;
    filledSize = other.filledSize;
    
    ptr = new ListItem[totalCapacity]();
    for (int i = 0; i < filledSize; i++) {
        ptr[i] = other.ptr[i];
    }
}

List::List(List&& other) {
    totalCapacity = other.totalCapacity;
    filledSize = other.filledSize;
    ptr = other.ptr;
    other.totalCapacity = 0;
    other.filledSize = 0;
    other.ptr = nullptr;
}

List::List(std::initializer_list<ListItem> items) {
    totalCapacity = items.size();
    filledSize = 0;
    ptr = new ListItem[totalCapacity];

    std::initializer_list<ListItem>::iterator it = items.begin();
    while (it != items.end()) {
        ptr[filledSize] = ListItem(*it);
        filledSize++;
        it++;
    }
}

List& List::operator=(const List& other) {
    if (this == &other) return *this;
    
    delete[] ptr;
    filledSize = other.filledSize;
    totalCapacity = other.totalCapacity;
    ptr = new ListItem[totalCapacity]();
    for (int i = 0; i < other.filledSize; i++) {
        ptr[i] = other.ptr[i];
    }
    return *this;
}

List& List::operator=(List &&other) {
    if (this == &other) return *this;

    delete[] ptr;
    filledSize = other.filledSize;
    totalCapacity = other.totalCapacity;
    ptr = other.ptr;
    other.totalCapacity = 0;
    other.filledSize = 0;
    other.ptr = nullptr;
    return *this;
}

List::~List() {
    delete[] ptr;
}

void List::growIfNeeded() {
    if (filledSize == totalCapacity) {
        setSize(2 * totalCapacity);
    }
}

int List::getSize() const {
    return totalCapacity;
}

void List::setSize(int size) {
    if (size < 0) size = 0;
    if (totalCapacity != size) {
        ListItem *newPtr = new ListItem[size]();
        int limit = (size < filledSize) ? size : filledSize;
        for (int i = 0; i < limit; i++) {
            newPtr[i] = ptr[i];
        }
        delete[] ptr;
        ptr = newPtr;
        totalCapacity = size;
        filledSize = limit;
    }
}

void List::add(int value) {
    growIfNeeded();
    ptr[filledSize] = ListItem(value);
    filledSize++;
}

void List::add(char value) {
    growIfNeeded();
    ptr[filledSize] = ListItem(value);
    filledSize++;
}

void List::add(bool value) {
    growIfNeeded();
    ptr[filledSize] = ListItem(value);
    filledSize++;
}

void List::add(float value) {
    growIfNeeded();
    ptr[filledSize] = ListItem(value);
    filledSize++;
}

void List::add(double value) {
    growIfNeeded();
    ptr[filledSize] = ListItem(value);
    filledSize++;
}

void List::add(std::string value) {
    growIfNeeded();
    ptr[filledSize] = ListItem(std::string(value));
    filledSize++;
}

void List::add(const char* value) {
    growIfNeeded();
    ptr[filledSize] = ListItem(value);
    filledSize++;
}

void List::add(const ListItem& value) {
    growIfNeeded();
    ptr[filledSize] = ListItem(value);
    filledSize++;
}

void List::remove() {
    if (filledSize > 0) {
        ptr[filledSize] = ListItem();
        filledSize--;
    }
}

void List::clear() {
    for (int i = 0; i < filledSize; i++) {
        ptr[i] = ListItem();
    }
    filledSize = 0;
}

void List::fullPrint() const {
    std::cout << "[";
    if (filledSize > 0) {
        for (int i = 0; i < filledSize - 1; i++) {
            std::cout << ptr[i].toString() << ", ";
        }
        std::cout << ptr[filledSize - 1].toString();
    }
    std::cout << "]\n";
}

void List::detailPrint() const {
    for (int i = 0; i < filledSize; i++) {
        std::cout << "Index: " << i 
        << " | Type: " << ptr[i].getTypeAsString() 
        << " | Value: " << ptr[i].toString() << std::endl;
    }
}

ListItem& List::operator[](int idx) const {
    return ptr[idx];
}