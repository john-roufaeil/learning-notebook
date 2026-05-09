#include "listitem.h"
#include <iostream>
#include <string>

ListItem::ListItem() {
    type = INT;
    intValue = 0;
}
ListItem::ListItem(int value) {
    type = INT;
    intValue = value;
}
ListItem::ListItem(char value) {
    type = CHAR;
    charValue = value;
}
ListItem::ListItem(bool value) {
    type = BOOL;
    boolValue = value;
}
ListItem::ListItem(float value) {
    type = FLOAT;
    floatValue = value;
}
ListItem::ListItem(double value) {
    type = DOUBLE;
    doubleValue = value;
}
ListItem::ListItem(std::string value) {
    type = STRING;
    stringValue = value;
}
ListItem::ListItem(const char* value) {
    type = STRING;
    stringValue = std::string(value);
}

ListItem::ListItem(const ListItem &other) {
    type = other.type;
    switch (other.type) {
        case INT: intValue = other.intValue; break;
        case CHAR: charValue = other.charValue; break;
        case BOOL: boolValue = other.boolValue; break;
        case FLOAT: floatValue = other.floatValue; break;
        case DOUBLE: doubleValue = other.doubleValue; break;
        case STRING: stringValue = other.stringValue; break;
    }
}

ListItem& ListItem::operator=(const ListItem &other) {
    if (this == &other) return *this;

    type = other.type;
    switch (other.type) {
        case INT: intValue = other.intValue; break;
        case CHAR: charValue = other.charValue; break;
        case BOOL: boolValue = other.boolValue; break;
        case FLOAT: floatValue = other.floatValue; break;
        case DOUBLE: doubleValue = other.doubleValue; break;
        case STRING: stringValue = other.stringValue; break;
    }
    return *this;
}

ListItem::ListItem(ListItem &&other) {
    type = other.type;
    switch(type) {
        case INT: intValue = other.intValue; break;
        case CHAR: charValue = other.charValue; break;
        case BOOL: boolValue = other.boolValue; break;
        case FLOAT: floatValue = other.floatValue; break;
        case DOUBLE: doubleValue = other.doubleValue; break;
        case STRING: stringValue = other.stringValue; break;
    }
}

ListItem& ListItem::operator=(ListItem &&other) {
    if (this == &other) return *this;
    type = other.type;
    switch(type) {
        case INT: intValue = other.intValue; break;
        case CHAR: charValue = other.charValue; break;
        case BOOL: boolValue = other.boolValue; break;
        case FLOAT: floatValue = other.floatValue; break;
        case DOUBLE: doubleValue = other.doubleValue; break;
        case STRING: stringValue = other.stringValue; break;
    }
    return *this;
}

ListItem::~ListItem() {}

ItemType ListItem::getType() const {
    return type;
}

std::string ListItem::getTypeAsString() const {  
    switch (type) {
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
    return intValue;
}
char ListItem::getChar() const {
    return charValue;
}
bool ListItem::getBool() const {
    return boolValue;
}
float ListItem::getFloat() const {
    return floatValue;
}
double ListItem::getDouble() const {
    return doubleValue;
}
std::string ListItem::getString() const {
    return stringValue;
}

std::string ListItem::toString() const {
    switch (type) {
        case INT:    return std::to_string(getInt());
        case CHAR:   return std::string("'") + getChar() + std::string("'");
        case BOOL:   return (getBool() ? "true" : "false");
        case FLOAT:  return std::to_string(getFloat()) + "f";
        case DOUBLE: return std::to_string(getDouble());
        case STRING: return std::string("\"") + getString() + std::string("\"");
    }
    return "UNKNOWN";
}

std::istream& operator>>(std::istream& is, ListItem& item) {
    std::string input;
    std::getline(is, input);
    
    int len = input.length();
    
    if (len == 0) {
        item = ListItem("");
    } else if (input == "true") {
        item = ListItem(true);
    } else if (input == "false") {
        item = ListItem(false);
    } else if (len == 1 && (input[0] < '0' || input[0] > '9')) {
        item = ListItem(input[0]);
    } else {
        bool hasDigits = false;
        bool hasOneDot = false;
        bool hasF = false;
        bool validNum = true;
        
        for (int i = 0; i < len; i++) {
            char c = input[i];
            if ((c >= '0' && c <= '9') || (c == '-' && i == 0)) {
                hasDigits = true;
            } else if (c == '.' && !hasOneDot) {
                hasOneDot = true;
            } else if (i == len - 1 && c == 'f') {
                hasF = true;
            } else {
                validNum = false;
                break;
            }
        }
        
        if (validNum && hasDigits && !hasOneDot && !hasF) {
            item = ListItem(std::stoi(input));
        } else if (validNum && hasDigits && hasOneDot && hasF) {
            item = ListItem(std::stof(input));
        } else if (validNum && hasDigits && hasOneDot && !hasF) {
            item = ListItem(std::stod(input));
        } else {
            item = ListItem(input);
        }
    }
    
    return is;
}
