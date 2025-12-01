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
        Stack(int size = 10) {
            if (size <= 0) {
                throw std::invalid_argument("Stack size cannot be non positive.");
            }
            this->size = size;
            this->topIdx = -1;
            this->ptr = new T[size];
        }

        Stack(const Stack<T> &other) {
            this->size = other.size;
            this->topIdx = other.topIdx;
            this->ptr = new T[this->size];
            for (int i = 0; i < this->topIdx + 1; i++) {
                this->ptr[i] = other.ptr[i];
            }
        }

        ~Stack() {
            delete[] this->ptr;
        }

        void push(T item) {
            if (topIdx + 1 < size) {
                topIdx++;
                ptr[topIdx] = item;
            } else {
                throw std::invalid_argument("Out of bound");
            }
        }

        T pop() {
            if (topIdx == -1) {
                throw std::invalid_argument("Out of bound");
            }
            T valToReturn = this->ptr[topIdx];
            topIdx--;
            return valToReturn;
        }

        T peek() const {
            if (topIdx == -1) {
                throw std::invalid_argument("Out of bound");
            }
            return this->ptr[topIdx];
        }

        bool isEmpty() const {
            return topIdx == -1;
        }

        bool isFull() const {
            return topIdx == this->size - 1;
        }

        void print() const {
            for (int i = 0; i < this->topIdx + 1; i++) {
                std::cout << this->ptr[i] << " ";
            }
            std::cout << std::endl;
        }

        Stack<T>& operator=(const Stack<T>& other) {
            if (this == &other) {
                return *this;
            }
            delete[] this->ptr;
            this->size = other.size;
            this->topIdx = other.topIdx;
            this->ptr = new T[this->size];
            for (int i = 0; i < this->topIdx + 1; i++) {
                this->ptr[i] = other.ptr[i];
            }
            return *this;
        }
};

#endif