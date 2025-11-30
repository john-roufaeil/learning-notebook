#ifndef STACK_H
#define STACK_H

class Stack {
    private:
        int *ptr;
        int size;
        int top;
    public:
        Stack(int size = 0);
        ~Stack();

        
        void push(int number);
        void pop();
        int peek();
        bool isEmpty();
        bool isFull();
        void print();
        int getSize();
};

#endif