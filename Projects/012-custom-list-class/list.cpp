#include "listitem.h"
#include "list.h"
#include <iostream>

List::List(int size) {
    this->totalCapacity = size;
    this->filledSize = 0;
    this->ptr = new ListItem[size];
}

List::List(const List& other) {
    this->totalCapacity = other.totalCapacity;
    this->filledSize = other.filledSize;
    
    this->ptr = new ListItem[totalCapacity];
    for (int i = 0; i < filledSize; i++) {
        this->ptr[i] = other.ptr[i];
    }
}

List::~List() {
    delete[] this->ptr;
}

List& List::operator=(const List& other) {
    if (this == &other) {
        return *this;
    }

    delete[] this->ptr;
    this->filledSize = other.filledSize;
    this->totalCapacity = other.totalCapacity;
    this->ptr = new ListItem[this->totalCapacity];
    for (int i = 0; i < other.filledSize; i++)
    {
        this->ptr[i] = other.ptr[i];
    }
    return *this;
}

void List::growIfNeeded() {
    if (this->filledSize == this->totalCapacity - 1) {
        this->setSize(2 * this->totalCapacity);
    }
}

int List::getSize() {
    return this->totalCapacity;
}

void List::setSize(int size) {
    if (size <= 0) size = 1;
    if (this->totalCapacity != size) {
        ListItem *newPtr = new ListItem[size];
        int limit = (size < this->filledSize) ? size : this->filledSize;
        for (int i = 0; i < limit; i++) {
            newPtr[i] = this->ptr[i];
        }
        delete[] this->ptr;
        this->ptr = newPtr;
        this->totalCapacity = size;
        this->filledSize = limit;
    }
}

void List::add(int value) {
    this->growIfNeeded();
    int indexToAddAt = this->filledSize;
    this->ptr[indexToAddAt] = ListItem(value);
    this->filledSize++;
}

void List::add(char value) {
    this->growIfNeeded();
    int indexToAddAt = this->filledSize;
    this->ptr[indexToAddAt] = ListItem(value);
    this->filledSize++;
}

void List::add(bool value) {
    this->growIfNeeded();
    int indexToAddAt = this->filledSize;
    this->ptr[indexToAddAt] = ListItem(value);
    this->filledSize++;
}

void List::add(float value) {
    this->growIfNeeded();
    int indexToAddAt = this->filledSize;
    this->ptr[indexToAddAt] = ListItem(value);
    this->filledSize++;
}

void List::add(double value) {
    this->growIfNeeded();
    int indexToAddAt = this->filledSize;
    this->ptr[indexToAddAt] = ListItem(value);
    this->filledSize++;
}

void List::add(std::string value) {
    this->growIfNeeded();
    int indexToAddAt = this->filledSize;
    this->ptr[indexToAddAt] = ListItem(value);
    this->filledSize++;
}

void List::remove() {
    if (this->filledSize > 0) {
        this->ptr[filledSize] = ListItem();
        this->filledSize--;
    }
}

void List::clear() {
    for (int i = 0; i < this->filledSize; i++) {
        this->ptr[i] = ListItem();
    }
}

// void List::fullPrint() {
//     for (int i = 0; i < this->filledSize; i++) {
//         std::cout << "Index: " << i << " Type: " << this->ptr[i].getType() << " Value: " << this->ptr[i]. << std::endl;
//     }
// }