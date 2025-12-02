#ifndef LIST_H
#define LIST_H

#include <string>

enum BoolValues {
    TRUE,
    FALSE
};

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
};

class List {
    private:
        ListItem *ptr;
        int filledSize;
        int totalCapacity;
        void grow();

    public:
        List(int size = 10);
        List(const List &other);
        ~List();

        int getSize();
        void setSize(int size);

        void add(int value);
        void add(char value);
        void add(bool value);
        void add(float value);
        void add(double value);
        void add(const std::string &value);

        void remove();
        void clear();
        void fullPrint();
        
        ItemType getType(int index);
        int getInt(int index);
        char getChar(int index);
        bool getBool(int index);
        float getFloat(int index);
        double getDouble(int index);
        std::string getString(int index);
        void printAt(int index);
};

#endif