#include <iostream>
#include <stdexcept>
#include "stack.h"

Stack::Stack(int size) {
    if (size <= 0) throw std::invalid_argument("Stack size cannot be non positive.");
    this->size = size;
    topIdx = -1;
    ptr = new int[size];
}

Stack::Stack(const Stack& other) {
    size = other.size;
    topIdx = other.topIdx;
    ptr = new int[size];
    for (int i = 0; i < topIdx + 1; i++) {
        ptr[i] = other.ptr[i];
    }
}

Stack& Stack::operator=(const Stack& other) {
    if (this == &other) return *this;
    delete[] ptr;
    size = other.size;
    topIdx = other.topIdx;
    ptr = new int[size];
    for (int i = 0; i < topIdx + 1; i++) {
        ptr[i] = other.ptr[i];
    }
    return *this;
}

Stack::Stack(Stack &&other) {
    size = other.size;
    topIdx = other.topIdx;
    ptr = other.ptr;
    other.size = 0;
    other.topIdx = -1;
    other.ptr = nullptr;
}

Stack& Stack::operator=(Stack &&other) {
    if (this == &other) return *this;
    delete[] ptr;
    size = other.size;
    topIdx = other.topIdx;
    ptr = other.ptr;
    other.size = 0;
    other.topIdx = -1;
    other.ptr = nullptr;
    return *this;
}

Stack::~Stack() {
    delete[] ptr;
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
        valToReturn = ptr[topIdx];
        topIdx--;
    } else {
        throw std::invalid_argument("Out of bound");
    }
    return valToReturn;
}

int Stack::peek() const {
    if (topIdx == -1) throw std::invalid_argument("Out of bound");
    return ptr[topIdx];
}

int Stack::count() const {
    return topIdx + 1;
}

bool Stack::isEmpty() const {
    return topIdx == -1;
}

bool Stack::isFull() const {
    return topIdx + 1 == size;
}

void Stack::print() const {
     if (!ptr) {
        std::cout << "ptr is not existing\n";
    } else {
        for (int i = 0; i < topIdx + 1; i++) {
            std::cout << ptr[i] << " ";
        }
        std::cout << std::endl;
    }
}