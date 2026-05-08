#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <stdexcept>

template <typename T>
class Stack {
    private:
        T *ptr;
        int size;
        int topIdx;
    public:
        Stack<T>(int size = 10) {
            if (size <= 0) throw std::invalid_argument("Stack size cannot be non positive.");
            this->size = size;
            topIdx = -1;
            ptr = new T[size];
        }

        Stack<T>(const Stack<T> &other) {
            size = other.size;
            topIdx = other.topIdx;
            ptr = new T[size];
            for (int i = 0; i < topIdx + 1; i++) {
                ptr[i] = other.ptr[i];
            }
        }

        Stack<T>& operator=(const Stack<T>& other) {
            if (this == &other) return *this;
            delete[] ptr;
            size = other.size;
            topIdx = other.topIdx;
            ptr = new T[size];
            for (int i = 0; i < topIdx + 1; i++) {
                ptr[i] = other.ptr[i];
            }
            return *this;
        }
        
        Stack<T>(Stack<T> &&other) {
            ptr = other.ptr;
            size = other.size;
            topIdx = other.topIdx;
            other.ptr = nullptr;
            other.size = 0;
            other.topIdx = -1;
        }

        Stack<T>& operator=(Stack<T> &&other) {
            if (this == &other) return *this;
            delete[] ptr;
            ptr = other.ptr;
            size = other.size;
            topIdx = other.topIdx;
            other.ptr = nullptr;
            other.size = 0;
            other.topIdx = -1;
            return *this;
        }

        ~Stack() {
            delete[] ptr;
        }

        void push(T item) {
            if (topIdx + 1 < size) {
                topIdx++;
                ptr[topIdx] = item;
            } else throw std::invalid_argument("Out of bound");
        }

        T pop() {
            if (topIdx == -1) throw std::invalid_argument("Out of bound");
            T valToReturn = ptr[topIdx];
            topIdx--;
            return valToReturn;
        }

        T peek() const {
            if (topIdx == -1) throw std::invalid_argument("Out of bound");
            return ptr[topIdx];
        }

        int count() const {
            return topIdx + 1;
        }

        bool isEmpty() const {
            return topIdx == -1;
        }

        bool isFull() const {
            return topIdx + 1 == size;
        }

        void print() const {
            for (int i = 0; i < topIdx + 1; i++) {
                std::cout << ptr[i] << " ";
            }
            std::cout << std::endl;
        }
};

#endif