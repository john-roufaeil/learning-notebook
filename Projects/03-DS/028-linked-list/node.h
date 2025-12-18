#ifndef NODE_H
#define NODE_H

template <typename T>
class Node {
    private:
        T data;
        *Node prev;
        *Node next;

    public:
        Node<T>(T data) {
            this.data = data;
            prev = NULL;
            next = NULL;
        }
};

#endif