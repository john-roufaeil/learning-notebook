#ifndef INSERTION_SORT_H
#define INSERTION_SORT_H

#include "linkedList.h"
#include "node.h"

void insertionSort(LinkedList<int> &ll) {
    Node<int> *head = ll.getHead();
    if (!head || !head->next) return;
    
    Node<int> *sorted = head;
    Node<int> *current = head->next;
    
    while (current) {
        Node<int> *nextNode = current->next;
        
        // find insertion position in sorted part
        if (current->data < head->data) {
            // insert at beginning
            current->prev->next = current->next;
            if (current->next) current->next->prev = current->prev;
            
            current->next = head;
            current->prev = nullptr;
            head->prev = current;
            head = current;
            ll.setHead(head);
        } else if (current->data >= sorted->data) {
            // already in correct position
            sorted = current;
        } else {
            // find position in sorted part
            Node<int> *search = sorted;
            while (search && search->data > current->data) {
                search = search->prev;
            }
            
            // remove current from its position
            current->prev->next = current->next;
            if (current->next) current->next->prev = current->prev;
            
            // insert after search
            current->next = search->next;
            current->prev = search;
            search->next->prev = current;
            search->next = current;
        }
        
        current = nextNode;
    }
}

#endif