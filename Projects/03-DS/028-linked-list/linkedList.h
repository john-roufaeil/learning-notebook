#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "node.h"

template <typename T>
class LinkedList {
    private:
        *Node head;
        *Node tail;

    public:
        LinkedList() {
            head = NULL;
            tail = NULL;
        }

        // 1. insert node after given node
        void insertAfter(T data, T afterData) {

        }
        // 2. insert node before given node
        void insertBefore(T data, T beforeData) {

        }

        // 3. get number of nodes
        int getCount() {
            int count = 0;
            *Node tmp = head;
            while (tmp != NULL) {
                count++;
                tmp = tmp->next;
            }
            return count;
        }

        // 4. get data in the given
        T getDataByIndex(int index) {
            *Node tmp = head;
            for (int i = 0; i < index; i++) {
                tmp = tmp->next;
            }
            return tmp->data;
        }

        // 5. Remove All Nodes
        void removeAll(int data) {

        }
};

#endif
