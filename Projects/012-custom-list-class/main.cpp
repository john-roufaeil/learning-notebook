#include "listitem.h"
#include "list.h"
#include <iostream>

int main() {
    List l1 = List(20);
    l1.add(5);
    l1.add("Hello");
    l1.add('c');
    l1.add(false);
    l1.add(3.1f);
    l1.add(5.5);
    l1.detailPrint();
    l1.fullPrint();

    std::cout << l1[0].getValueToString() << std::endl;
    std::cout << l1[1].getValueToString() << std::endl;
    std::cout << l1[2].getValueToString() << std::endl;
    std::cout << l1[3].getValueToString() << std::endl;
    std::cout << l1[4].getValueToString() << std::endl;
    std::cout << l1[5].getValueToString() << std::endl;

    l1[0] = 3939;
    l1.detailPrint();
}