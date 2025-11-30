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

    // String s1 = String("hello");
    // s1.fullPrint();
    // String s2 = s1;
    // s1 = s1;
    // s1[0] = 'b';
    // s1.fullPrint();
    // std::cout << String("Hello") << std::endl;
    // s1 = "jhndsfg jklvhndfsag";
    // s1.fullPrint();

    // String s1 = "hi";
    // String s2 = String("hello");
    // s1 = "newwwHI";
    // s2 = String("i");
    // s1.fullPrint();
    // s2.fullPrint();
    // String s3 = String();
    // s3.fullPrint();
    
    // std::cout << s4.length() << std::endl;
    // s4.setSize(5);
    // std::cout << s4.length() << std::endl;
    // s4.fullPrint();
    // s4.fullPrint();
    String s4 = String(5);
    s4.setStr("balabizo");
    s4.resize(10);
    s4.resize(2);
    s4.fullPrint();
    s4.clear();
    s4.fullPrint();
    s4.resize(3);
    s4[0] = 'h';
    s4[1] = s4[0];
    s4.fullPrint();
    return 0;
}