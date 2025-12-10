#ifndef STACK_H
#define STACK_H

class Stack {
    private:
        int *ptr;
        int size;
        int topIdx;
    public:
        Stack(int size = 10);
        Stack(const Stack &other);
        Stack& operator=(const Stack &other);
        Stack(Stack &&other); 
        Stack& operator=(Stack &&other);
        ~Stack();

        void push(int number);
        int pop();
        int peek() const;
        int count() const;
        bool isEmpty() const;
        bool isFull() const;
        void print() const;
};

#endif