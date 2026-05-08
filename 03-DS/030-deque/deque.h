#include <iostream>

/*
Simple Queue
    Delete ===> Front
    Insert ===> Rear
    problems : deleted spaces cannot be reused
    solution : Circular Queue
==================================================
Circular Queue
    Delete ===> Front
    Insert ===> Rear
==================================================
Priority Queue
    Delete ===> Priority, Front
    Insert ===>  Rear
==================================================
Double Ended Queue
    Input Restricted  [One Insertion , Both Delete]
    insertRear();
    deleteFront();
    deleteRear();

    Output Restricted
    insertRear();
    insertFront();
    deleteFront();
==================================================*/

class Deque{
    private :
        int *items;
        int rear;
        int capacity;
        int size;

    public:
        int front;
        
        Deque(int capacity){
            rear=-1;
            front=-1;
            size=0;
            this->capacity=capacity;
            items=new int[capacity];
        }
        ~Deque(){
            delete [] items;
        }

        void resize() {
            int newCapacity = capacity * 2;
            int *newItems = new int[newCapacity];

            for (int i = front; i < capacity; i++) {
                newItems[i - front] = items[i];
            }
            for (int i = 0; i < front; i++) {
                newItems[capacity - front + i] = items[i];
            }
            delete[] items;
            items = newItems;
            front = 0;
            rear = capacity - 1;
            capacity = newCapacity;
        }

        void insertRear(int item){
            if(size == capacity)
                resize();

            // Setting Rear and Front
            if(rear == -1 && front == -1) { //First Element
                rear = 0;
                front = 0;
            } else if (rear == capacity - 1) { // Wrap Around
                rear = 0;
            } else { // Normal Case
                rear++;
            }
            
            items[rear] = item;
            size++;
        }

        void insertFront(int item) {
            if (size == capacity)
                resize();
            
            // Setting Front and Rear
            if (rear == -1 && front == -1) { // First Element
                rear = 0;
                front = 0;
            } else if (front == 0) { // Wrap Around
                front = capacity - 1;
            } else { // Normal Case
                front--;
            }

            items[front] = item;
            size++;
        }
    
    int deleteFront() {
        if (size == 0) {
            std::cout << "Empty Queue" ; 
            return -1;
        }

        int tmp = items[front];
        items[front] = -1;
        size--;
        
        if(front == rear) { // Only one element
            front = -1;
            rear = -1;
        } else if (front == capacity - 1) { // Wrap Around
            front = 0;
        } else { // Normal Case
            front++;
        }
        return tmp;
    }

    int deleteRear() {
        if (size == 0) {
            std::cout<<"Empty Queue" ; 
            return -1;
        }

        int tmp = items[rear];
        items[rear]= -1;
        size--;
        
        if(front == rear) { // Only one element
            front = -1;
            rear = -1;
        } else if (rear == 0) { // Wrap Around
            rear = capacity - 1;
        } else { // Normal Case
            rear--;
        }
        return tmp;
    }

    void display() {
        // Empty
        if(front == -1) {
            std::cout << "Empty Queue\n";
            return ;
        }
        
        std::cout << "\n========================================\n";
        if(front <= rear) { // Normal Case
            for (int i = front ; i<= rear; i++) {
                std::cout << items[i] << "  ";
            }
        } else { // Wrap Around
            for (int i= front; i< capacity; i++){
                std::cout << items[i] << "  ";
            }
            for (int i= 0; i <= rear; i++){
                std::cout << items[i] << "  ";
            }
        }
        std::cout << "\n========================================\n";
    }

    void printSizeCapacity() {
        std::cout << "Size: " << size << ", Capacity: " << capacity << std::endl;
    }
};
