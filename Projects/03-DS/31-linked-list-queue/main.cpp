#include <iostream>
#include "linkedListQueue.h"
using namespace std;

int main()
{
    QueueLinkedList q;
    q.add(10);
    q.add(20);
    q.add(30);
    q.add(40);
    q.add(50);
    q.add(60);
    q.display();
    try{
        q.remove();//10
        q.remove(); // 20
        q.remove(); //30
        q.remove(); //40

    }catch (Exception e){
        cout<<e.name;
    }
     q.display();//50 60
    return 0;
}