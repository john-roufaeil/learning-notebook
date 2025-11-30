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
    
    // String s4 = String(5);
    // std::cout << s4.length() << std::endl;
    // s4.setSize(5);
    // std::cout << s4.length() << std::endl;
    // s4.fullPrint();
    // s4.fullPrint();
    // s4.setStr("balabizo");
    // s4.resize(10);
    // s4.resize(2);
    // s4.fullPrint();
    // s4.clear();
    // s4.fullPrint();
    // s4.resize(3);
    // s4[0] = 'h';
    // s4[1] = s4[0];
    // s4.fullPrint();

    String s1 = "hello";
    String s2 = " world";
    s1.fullPrint();
    s1.append(s2);
    s1.fullPrint();

    s1+="!";
    s1.fullPrint();
    String s3 = String("hello world!");
    String s4 = String("hi");
    // std::cout << (s1 == s2);
    if (s1 == s3)
        std::cout << "yes";
    else
        std::cout << "no";
    if (s1 == s4)
        std::cout << "yes";
    else
        std::cout << "no";
    if (s3 == s1)
        std::cout << "yes";
    else
        std::cout << "no";
    if (s4 == s1)
        std::cout << "yes";
    else
        std::cout << "no";
    if (s3 == s4)
        std::cout << "yes";
    else
        std::cout << "no";
    if (s3 < s4)
        std::cout << "less";
    else
        std::cout << "not less";
    return 0;
}