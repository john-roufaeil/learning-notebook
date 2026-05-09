#ifndef LINEAR_SEARCH_H
#define LINEAR_SEARCH_H

#include "node.h"
#include "linkedList.h"

Node<int>* linearSearch(LinkedList<int> &l, int target) {
    Node<int> *ptr = l.getHead();
    while (ptr) {
        if (ptr->data == target) return ptr;
        ptr = ptr->next;
    }
    return nullptr;
}

#endif