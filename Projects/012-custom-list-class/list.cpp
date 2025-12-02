#include "list.h"

// ListItem ---

ListItem::ListItem() {
    this->type = INT;
    this->intValue = 0;
}

ListItem::ListItem(int value) {
    this->type = INT;
    this->intValue = value;
}
ListItem::ListItem(char value) {
    this->type = CHAR;
    this->charValue = value;
}
ListItem::ListItem(bool value) {
    this->type = BOOL;
    this->boolValue = value;
}
ListItem::ListItem(float value) {
    this->type = FLOAT;
    this->floatValue = value;
}
ListItem::ListItem(double value) {
    this->type = DOUBLE;
    this->doubleValue = value;
}
ListItem::ListItem(std::string value) {
    this->type = STRING;
    this->stringValue = value;
}

ListItem::ListItem(const ListItem &other) {
    this->type = other.type;
    switch (other.type) {
        case INT:
            this->intValue = other.intValue;
            break;
        case CHAR:
            this->charValue = other.charValue;
            break;
        case BOOL:
            this->boolValue = other.boolValue;
            break;
        case FLOAT:
            this->floatValue = other.floatValue;
            break;
        case DOUBLE:
            this->doubleValue = other.doubleValue;
            break;
        case STRING:
            this->stringValue = other.stringValue;
            break;
    }
}

ListItem& ListItem::operator=(const ListItem &other) {
    if (this == &other) return *this;

    this->type = other.type;
    switch (other.type) {
        case INT:
            this->intValue = other.intValue;
            break;
        case CHAR:
            this->charValue = other.charValue;
            break;
        case BOOL:
            this->boolValue = other.boolValue;
            break;
        case FLOAT:
            this->floatValue = other.floatValue;
            break;
        case DOUBLE:
            this->doubleValue = other.doubleValue;
            break;
        case STRING:
            this->stringValue = other.stringValue;
            break;
    }
    return *this;
}

// List ---

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

