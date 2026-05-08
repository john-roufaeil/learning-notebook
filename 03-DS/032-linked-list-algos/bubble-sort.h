#ifndef BUBBLE_SORT_H
#define BUBBLE_SORT_H

#include "node.h"
#include "linkedList.h"
#include <iostream>

void bubbleSort(LinkedList<int> ll) {
    Node<int> *head = ll.getHead();
    Node<int> *ptr = head;
    while(ptr) {
        Node<int> *innerPtr = head;
        while(innerPtr && innerPtr->next) {
            if (innerPtr->data > innerPtr->next->data) {
                int temp = innerPtr->data;
                innerPtr->data = innerPtr->next->data;
                innerPtr->next->data = temp;
            }
            innerPtr =  innerPtr->next;
        }
        ptr = ptr->next;
    }
}

#endif
