#include <iostream>
#include <stdexcept>
#include "stack.h"

Stack::Stack(int size) {
    if (size <= 0) {
        throw std::invalid_argument("Stack size cannot be non positive.");
    }
    this->size = size;
    this->topIdx = -1;
    this->ptr = new int[size];
}

Stack::Stack(const Stack& other) {
    this->size = other.size;
    this->topIdx = other.topIdx;
    this->ptr = new int[this->size];
    for (int i = 0; i < this->topIdx; i++) {
        this->ptr[i] = other.ptr[i];
    }
}

Stack::~Stack() {
    delete[] this->ptr;
}

void Stack::push(int num) {
    if (topIdx + 1 < size) {
        topIdx++;
        ptr[topIdx] = num;
    } else {
        throw std::invalid_argument("Out of bound");
    }
}

int Stack::pop() {
    int valToReturn = 0;
    if (topIdx != -1) {
        valToReturn = this->ptr[topIdx];
        topIdx--;
    } else {
        throw std::invalid_argument("Out of bound");
    }
    return valToReturn;
}

int Stack::peek() {
    if (topIdx == -1) {
        throw std::invalid_argument("Out of bound");
    }
    return this->ptr[topIdx];
}

bool Stack::isEmpty() {
    return topIdx == -1;
}

bool Stack::isFull() {
    return topIdx == this->size - 1;
}

void Stack::print() {
    for (int i = 0; i < this->topIdx + 1; i++) {
        std::cout << this->ptr[i] << " ";
    }
    std::cout << std::endl;
}

Stack& Stack::operator=(const Stack& other) {
    if (this == &other) {
        return *this;
    }
    delete[] this->ptr;
    this->size = other.size;
    this->topIdx = other.topIdx;
    this->ptr = new int[this->size];
    for (int i = 0; i < this->topIdx + 1; i++) {
        this->ptr[i] = other.ptr[i];
    }
    return *this;
}