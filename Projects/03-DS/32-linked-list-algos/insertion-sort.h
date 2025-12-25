#ifndef INSERTION_SORT_H
#define INSERTION_SORT_H

#include "linkedList.h"
#include "node.h"

void insertionSort(LinkedList<int> ll) {
    Node<int> *head = ll.getHead();
    Node<int> *lastSorted = head;
    
    while (lastSorted) {
        Node<int> *min = lastSorted;
        Node<int> *innerPtr = lastSorted;
        while (innerPtr) {
            if (innerPtr->data < min->data) {
                min = innerPtr;
            }
            innerPtr = innerPtr->next;
        }
        if (min != lastSorted) {
            Node<int> *tmp = lastSorted;
            lastSorted->next->prev = min;
            min->prev->next = lastSorted;
            tmp = min->next;
            min->next = lastSorted->next;
            lastSorted->next = tmp;
            tmp = min->prev;
            min->prev = lastSorted->prev;
            lastSorted->prev = tmp;
        }
        lastSorted = lastSorted->next;
    }
}
#endif