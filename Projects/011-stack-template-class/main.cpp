#include <iostream>
#include <string>
#include "stack.h"

int main() {
    Stack<int> s(5);

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
    std::cout << "Is empty? " << s.isEmpty() << "\n";
    std::cout << "Is full? " << s.isFull() << "\n";

    Stack s2 = Stack<int>();
    std::cout << "New stack created: ";
    s2.print();
    s2 = s;
    std::cout << "New stack copied: ";
    s2.print();

    Stack<std::string> s3;
    std::cout << "String stack: ";
    s3.print();
    s3.push("Hello");
    s3.push("World");
    s3.push("I'm");
    s3.push("a");
    s3.push("Template");
    s3.push("Stack");
    s3.print();

    std::cout << "Copy constructor string: ";
    Stack<std::string> s4 = s3;
    s4.print();

    Stack<std::string> s5(std::move(s4));
    std::cout << "s5: "; s5.print();

    Stack<std::string> s6;
    s6 = std::move(s3);
    std::cout << "s6: "; s6.print();
    std::cout << "s6 count: " << s6.count() << std::endl;

    return 0;
}
