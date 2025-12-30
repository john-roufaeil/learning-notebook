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
        Node *next;
        Node(int data) {
            this-> data = data;
            this-> next = NULL;
        }
};

class QueueLinkedList{
    Node *head;
    Node *tail;

    public:
        QueueLinkedList() {
            head = NULL;
            tail = NULL;
        }

        void add(int data) {
            Node *newNode = new Node(data);
            if (tail == NULL) {
                head = tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
        }
        
        int remove() {
            if (head == NULL ) {
                Exception e;
                e.code=404;
                e.name="Queue is Empty";
                throw e;
            }

            Node *tmp = head;
            head = head-> next;
            if (head == NULL) {
                tail = NULL;
            }
            int data = tmp-> data;
            delete tmp;
            return data;
        }

        void display() {
            Node *current = head;
            cout << "\n============================\n";
            while(current != NULL) {
                cout << current-> data << "  ";
                current = current-> next;
            }
            cout << "\n============================\n";
        }
};
