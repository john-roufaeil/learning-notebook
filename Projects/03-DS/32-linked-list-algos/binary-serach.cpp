#include "node.h"
#include "linkedList.h"
#include <iostream>

int calculateNodesCount(Node<int>* start, Node<int>* end);

Node<int>* binarySearch(LinkedList<int> &l, int target) {
    Node<int> *left = l.getHegit statusad();
    Node<int> *right = l.getTail();

    Node<int> *mid = left;
    for (int i = 0; i < l.getSize() / 2; i++) {
        mid = mid->next;
    }

    while (left && right && right->next != left) {
        if (mid->data > target) {
            right = mid->prev;
            mid = left;
            for (int i = 0; i < calculateNodesCount(left, right) / 2; i++) {
                mid = mid->next;
            }
        } else if (mid->data < target) {
            left = mid->next;
            for (int i = 0; i < calculateNodesCount(left, right) / 2; i++) {
                mid = mid->next;
            }
        } else {
            return mid;
        }
    }
    return nullptr;
}

int calculateNodesCount(Node<int>* start, Node<int>* end) {
    if (!start || !end)
        return 0;
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