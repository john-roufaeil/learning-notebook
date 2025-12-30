#include "linkedListStack.h"

int main()
{
    StackLinkedList s;
    s.push(10);
    s.push(20);
    s.push(30);
    s.push(40);
    s.push(50);
    s.push(60);
    s.display();
    try{
        s.pop();//60
        s.pop(); // 50
        s.pop(); //40
        s.pop(); //30

    }catch (Exception e){
        cout<<e.name;
    }
     s.display();//20 10
    //cout << "Hello world!" << endl;
    return 0;
}