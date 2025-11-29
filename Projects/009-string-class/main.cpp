#include "string.h"
#include <iostream>

int main() {
    // String s = String(5);
    // String s1 = String("hi");
    // std::cout << s << std::endl;
    // std::cout << s1 << std::endl;
    // s.fullPrint();
    // s1.fullPrint();
    // s1.setStr("hello");
    // s1.fullPrint();
    // std::cout << std::endl; 
    // std::cout << std::endl; 
    // std::cout << s1.getSize();
    // std::cout << std::endl; 
    // std::cout << s1[0];
    // std::cout << s1[1];
    // std::cout << s1[2];
    // std::cout << s1[3];
    // std::cout << s1[4];
    // std::cout << s1[5];
    // std::cout << std::endl; 
    // s1[0] = 'b';
    // s1.fullPrint();
    String s1 = String("hello");
    s1.fullPrint();
    String s2 = s1;
    s1 = s1;
    s1[0] = 'b';
    s1.fullPrint();
    return 0;
}