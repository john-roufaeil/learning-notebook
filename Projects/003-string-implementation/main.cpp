#include <iostream>
#include <string.h>
#include "library.h"

using namespace std;

int main() {
    cout << "String Implementation Project" << endl;
    print_divider();

    char testStr1[] = "Hello, World!";
    char testStr2[] = "Hello, World!";
    char testStr3[] = "Hello, World?";
    char testStr4[] = "Hello, World";
    char testStr5[] = "ANOTHER $TR!NG";
    char testStr6[] = "This is a Sample String";
    char testStr7[20] = {'H', 'e', 'l', 'l', 'o', '\0'};
    char testStr8[6] = "World";
    char testStr9[] = "Sample";
    char testStr10[] = "Text";

    cout << "Implemented Length of '" << testStr1 << "' is: " << str_len(testStr1) << endl;
    cout << "Expected Length is: " << strlen(testStr1) << endl;
    print_divider();

    cout << "Implemented Comparison of '" << testStr1 << "' and '" << testStr3 << "' is: " << str_cmp(testStr1, testStr3) << endl;
    cout << "Expected Comparison is: " << strcmp(testStr1, testStr3) << endl;
    cout << "Implemented Comparison of '" << testStr1 << "' and '" << testStr2 << "' is: " << str_cmp(testStr1, testStr2) << endl;
    cout << "Expected Comparison is: " << strcmp(testStr1, testStr2) << endl;
    cout << "Implemented Comparison of '" << testStr1 << "' and '" << testStr4 << "' is: " << str_cmp(testStr1, testStr4) << endl;
    cout << "Expected Comparison is: " << strcmp(testStr1, testStr4) << endl;
    print_divider();

    cout << "Original String for Lowercase Conversion: " << testStr5 << endl;
    to_lowercase(testStr5);
    cout << "After Lowercase Conversion: " << testStr5 << endl;
    print_divider();

    cout << "Original String for Uppercase Conversion: " << testStr6 << endl;
    to_uppercase(testStr6);
    cout << "After Uppercase Conversion: " << testStr6 << endl;
    print_divider();

    cout << "Original String for Concatenating " << testStr7 << " and " << testStr8 << endl;
    cout << "After Concatenation: ";
    str_concat(testStr7, testStr8, sizeof(testStr7)/sizeof(testStr7[0]));
    cout << testStr7 << endl;

    cout << "Implemented Copying of '" << testStr10 << "' into '" << testStr9 << "' results in: ";
    str_copy(testStr9, testStr10);
    cout << testStr9 << endl;

    return 0;
}
