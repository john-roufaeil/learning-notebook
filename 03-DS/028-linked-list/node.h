#ifndef NODE_H
#define NODE_H

template <typename T>
class Node {
    public:
        T data;
        Node<T>* prev;
        Node<T>* next;
        Node(T data) {
            this->data = data;
            prev = nullptr;
            next = nullptr;
        }
};

#endif