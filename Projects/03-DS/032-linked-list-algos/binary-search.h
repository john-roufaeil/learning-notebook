#ifndef BINARY_SEARCH_H
#define BINARY_SEARCH_H

#include "node.h"
#include "linkedList.h"
#include <iostream>

Node<int> *getMiddle(Node<int> *left, Node<int> *right);

Node<int> *binarySearch(LinkedList<int> &l, int target) {
    Node<int> *left = l.getHead();
    Node<int> *right = l.getTail();
    if (left == right) {
        return (left->data == target) ? left : nullptr;
    }
    // Node<int> *mid = left;
    // for (int i = 0; i < l.getSize() / 2; i++) {
    //     mid = mid->next;
    // }
    Node<int> *mid = getMiddle(left, right);

    while (left && right && left != right->next) {
        if (mid->data > target) {
            right = mid->prev;
            // mid = left;
            // for (int i = 0; i < calculateNodesCount(left, right) / 2; i++) {
            //     mid = mid->next;
            // }
            mid = getMiddle(left, right);
        } else if (mid->data < target) {
            left = mid->next;
            // mid = left;
            // for (int i = 0; i < calculateNodesCount(left, right) / 2; i++) {
            //     mid = mid->next;
            // }
            mid = getMiddle(left, right);   
        } else {
            return mid;
        }
    }
    return nullptr;
}

int calculateNodesCount(Node<int>* start, Node<int>* end) {
    if (!start || !end) return 0;
    
    int count = 1; // Count the start node
    Node<int>* curr = start;
    while (curr != end) {
        count++;
        curr = curr->next;
        if (!curr) return 0; // end not reachable from start
    }
    return count;
}

Node<int>* getMiddle(Node<int>* left, Node<int>* right) {
    Node<int>* slow = left;
    Node<int>* fast = left;

    while (fast != right && fast->next != right && fast->next != nullptr) {
        fast = fast->next->next;
        slow = slow->next;
    }
    return slow;
}

#endif
