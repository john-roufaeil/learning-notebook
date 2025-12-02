#include "listitem.h"
#include <iostream>
#include <string>

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

ItemType ListItem::getType() {
    return this->type;
}

int ListItem::getInt() {
    return this->intValue;
}
char ListItem::getChar() {
    return this->charValue;
}
bool ListItem::getBool() {
    return this->boolValue;
}
float ListItem::getFloat() {
    return this->floatValue;
}
double ListItem::getDouble() {
    return this->doubleValue;
}
std::string ListItem::getString() {
    return this->stringValue;
}
