#include "listitem.h"
#include "list.h"
#include <iostream>

int main() {
    List l1 = {};
    l1.add(5);
    l1.add("Hello");
    l1.add('c');
    l1.add(false);
    std::cout << l1.getSize() << std::endl;
    l1.add(3.1f);
    std::cout << l1.getSize() << std::endl;
    
    l1.detailPrint();
    l1.fullPrint();

    std::cout << l1[0].toString() << std::endl;
    std::cout << l1[1].toString() << std::endl;
    std::cout << l1[2].toString() << std::endl;
    std::cout << l1[3].toString() << std::endl;
    std::cout << l1[4].toString() << std::endl;

    l1[0] = 3939;
    l1.detailPrint();

    std::cout << "List l2: \n";
    List l2 = {
        1,
        2.0f,
        "Hi",
        true,
        2.54};
    List l3 = {"Hi", 2, 9.0f, 38.43, 'c', true};
    l3.detailPrint();
    l3.fullPrint();
    std::cout << l3.getSize();
    l3.add("Oops");
    l3.fullPrint();
    std::cout << l3.getSize();
    l3.remove();
    l3.fullPrint();
    std::cout << l3.getSize();
    l3.setSize(3);
    l3.fullPrint();
    std::cout << l3.getSize();
}