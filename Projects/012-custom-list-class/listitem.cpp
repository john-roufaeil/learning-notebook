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
ListItem::ListItem(const char* value) {
    this->type = STRING;
    this->stringValue = std::string(value);
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

std::string ListItem::getTypeAsString() {  
    switch (this->type) {
        case INT:    return "INT";
        case CHAR:   return "CHAR";
        case BOOL:   return "BOOL";
        case FLOAT:  return "FLOAT";
        case DOUBLE: return "DOUBLE";
        case STRING: return "STRING";
        default:     return "UNKNOWN";
    }
}

int ListItem::getInt() const {
    return this->intValue;
}
char ListItem::getChar() const {
    return this->charValue;
}
bool ListItem::getBool() const {
    return this->boolValue;
}
float ListItem::getFloat() const {
    return this->floatValue;
}
double ListItem::getDouble() const {
    return this->doubleValue;
}
std::string ListItem::getString() const {
    return this->stringValue;
}

std::string ListItem::getValueToString() const {
    switch (this->type) {
        case INT:    return std::to_string(intValue);
        case CHAR:   return std::string(1, charValue);
        case BOOL:   return boolValue ? "true" : "false";
        case FLOAT:  return std::to_string(floatValue) + "f";
        case DOUBLE: return std::to_string(doubleValue);
        case STRING: return stringValue;
    }
}
