#include <iostream>
#include "stack.h"

int main() {
    Stack s(5);

    s.push(10);
    s.push(20);
    s.push(30);

    std::cout << "Stack after pushes: ";
    s.print();

    std::cout << "Popped: " << s.pop() << "\n";
    std::cout << "Popped: " << s.pop() << "\n";
    std::cout << "Popped: " << s.pop() << "\n";

    std::cout << "Stack after pop: ";
    s.print();

    std::cout << "Is empty? " << s.isEmpty() << "\n";
    std::cout << "Is full? " << s.isFull() << "\n";

    s.push(40);
    s.push(50);
    s.push(60);
    s.push(40);
    s.push(50);

    std::cout << "Stack after filling: ";
    s.print();
    std::cout << s.count() << std::endl;
    std::cout << "Is empty? " << s.isEmpty() << "\n";
    std::cout << "Is full? " << s.isFull() << "\n";

    Stack s2 = Stack();
    std::cout << "New stack created: ";
    s2.print();
    s2 = s;
    std::cout << "New stack copied: ";
    s2.print();


    Stack a(5);
    a.push(10);
    a.push(20);
    a.push(30);

    std::cout << "Stack a before move: ";
    a.print();

    Stack b(std::move(a));

    std::cout << "Stack a after move: ";
    a.print();

    std::cout << "Stack b after move: ";
    b.print();   

    return 0;
}
