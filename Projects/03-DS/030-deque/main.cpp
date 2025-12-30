#include "deque.h"

int main() {
    Deque q(5);
    // q.display();
    // q.printSizeCapacity();
    q.insertFront(10);
    q.insertRear(20);
    // q.display();
    // q.printSizeCapacity();
    q.insertRear(30);
    q.insertFront(8);
    q.insertRear(35);
    // q.display();
    // q.printSizeCapacity();
    q.insertFront(5);
    q.deleteFront();
    q.deleteRear();
    q.display();
    q.printSizeCapacity();

    return 0;
}
