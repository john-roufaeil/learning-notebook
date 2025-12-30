#include <iostream>
using namespace std;

class Exception{
    public :
        int code;
        char *name ;
};

class Node{
    public :
        int data;
        Node *prev;
        Node(int data) {
            this-> data = data;
            this-> prev = NULL;
        }
};

class StackLinkedList{
    Node * tail;

    public :
        StackLinkedList() {
            tail = NULL;
        }

        void push(int data) {
            Node *newNode = new Node(data);
            newNode-> prev = tail;
            tail = newNode;
        }
        
        int pop() {
            if(tail == NULL ){
                Exception e;
                e.code=404;
                e.name="Stack Empty";
                throw e;
            }
            Node *tmp = tail;
            tail = tail-> prev;
            int data = tmp-> data;
            delete tmp;
            return data;
        }

        void display() {
            Node *current = tail;
            cout << "\n ============================ \n";
            while(current != NULL) {
                cout << current-> data << "  ";
                current = current-> prev;
            }
            cout << "\n ============================ \n";
        }
};
