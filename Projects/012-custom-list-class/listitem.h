#ifndef LIST_ITEM_H
#define LIST_ITEM_H
#include <string>

enum ItemType {
    INT,
    CHAR,
    BOOL,
    FLOAT,
    DOUBLE,
    STRING
};

class ListItem {
    public:
        ItemType type;
        union {
            int intValue;
            char charValue;
            bool boolValue;
            float floatValue;
            double doubleValue;
        };
        std::string stringValue;
        
        ListItem();
        ListItem(int value);
        ListItem(char value);
        ListItem(bool value);
        ListItem(float value);
        ListItem(double value);
        ListItem(std::string value);
        ListItem(const ListItem &other);
        ListItem& operator=(const ListItem &other);
        ItemType getType();

        int getInt();
        char getChar();
        bool getBool();
        float getFloat();
        double getDouble();
        std::string getString();
};

#endif
