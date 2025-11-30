#ifndef STACK_H
#define STACK_H

class Stack {
    private:
        int *ptr;
        int size;
        int topIdx;
    public:
        Stack(int size = 10);
        ~Stack();

        void push(int number);
        int pop();
        int peek();
        bool isEmpty();
        bool isFull();
        void print();
};

#endif