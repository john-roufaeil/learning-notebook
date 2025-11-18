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
    char testStr7[] = {'H', 'e', 'l', 'l', 'o', '\0'};

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

    

    return 0;
}
