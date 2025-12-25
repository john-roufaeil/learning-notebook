#ifndef SELECTION_SORT_H
#define SELECTION_SORT_H

#include "linkedList.h"
#include "node.h"

void selectionSort(LinkedList<int> &ll) {
    Node<int> *head = ll.getHead();
    if (!head || !head->next) return;
    
    Node<int> *lastSorted = head;
    
    while (lastSorted && lastSorted->next) {
        // find min node in unsorted part
        Node<int> *min = lastSorted;
        Node<int> *innerPtr = lastSorted->next;
        while (innerPtr) {
            if (innerPtr->data < min->data) {
                min = innerPtr;
            }
            innerPtr = innerPtr->next;
        }

        // swap min with lastSorted
        if (lastSorted->next == min) { // nodes are adjacent
            if (min->next && min->next->prev) min->next->prev = lastSorted;
            if (lastSorted->prev && lastSorted->prev->next) lastSorted->prev->next = min;
            min->prev = lastSorted->prev;
            lastSorted->prev = min;
            lastSorted->next = min->next;
            min->next = lastSorted;
        } else if (lastSorted != min) { // nodes are not adjacent
            Node<int> *tmp = lastSorted;
            if (lastSorted->prev && lastSorted->prev->next) lastSorted->prev->next = min;
            if (min->next && min->next->prev) min->next->prev = lastSorted;
            if (lastSorted->next && lastSorted->next->prev) lastSorted->next->prev = min;
            if (min->prev && min->prev->next) min->prev->next = lastSorted;
            tmp = min->next;
            min->next = lastSorted->next;
            lastSorted->next = tmp;
            tmp = min->prev;
            min->prev = lastSorted->prev;
            lastSorted->prev = tmp;
        }

        if (lastSorted == head) {
            ll.setHead(min);
                head = min;
        }
        lastSorted = min->next;
    }
}
#endif