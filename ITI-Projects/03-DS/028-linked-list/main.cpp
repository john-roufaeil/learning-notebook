#include "linkedList.h"
#include <string>

int main() {
    LinkedList<int> ll;
    int count1 = ll.getCount();
    ll.insertAfter(5,5);
    ll.insertBefore(1,5);
    ll.insertAfter(15,5);
    ll.insertBefore(10,15);
    ll.insertAfter(20,15);
    ll.print();
    std::cout << "count: " << ll.getCount() << "\n";
    std::cout << ll.getDataByIndex(0) << "\n";
    std::cout << ll.getDataByIndex(1) << "\n";
    std::cout << ll.getDataByIndex(2) << "\n";
    std::cout << ll.getDataByIndex(3) << "\n";
    std::cout << ll.getDataByIndex(4) << "\n";
    // std::cout << ll.getDataByIndex(5) << "\n";
    ll.insertAfter(100,1234);
    ll.print();
    ll.insertBefore(99,100);
    ll.print();
    ll.insertBefore(0,1);
    ll.print();
    ll.insertAfter(101,100);
    ll.print();
    ll.removeFirst(0);
    ll.print();
    ll.removeFirst(5);
    ll.print();
    ll.removeFirst(15);
    ll.print();
    ll.removeFirst(101);
    ll.print();
    ll.removeFirst(0);
    ll.print();
    // ll.insert(5);
    // ll.insert(5);
    // ll.insert(5);
    // ll.insert(5);
    // ll.insert(5);
    // ll.insert(15);
    // ll.insert(15);
    // ll.print();
    // ll.removeFirst(15);
    // ll.print();
    // ll.removeAll(5);
    // ll.print();
}