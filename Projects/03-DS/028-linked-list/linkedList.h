#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "node.h"
#include <iostream>

template <typename T>
class LinkedList {
    private:
        Node<T>* head;
        Node<T>* tail;

    public:
        LinkedList() {
            head = nullptr;
            tail = nullptr;
        }

        ~LinkedList() {
            Node<T>* nodeIt = head;
            while (nodeIt) {
                Node<T>* next = nodeIt->next;
                delete nodeIt;
                nodeIt = next;
            }
        }

        Node<T>* getHead() {
            return head;
        }
        Node<T>* getTail() {
            return tail;
        }

        void insert(T data) {
            Node<T> *newNode = new Node(data);
            if (!head) {
                head = tail = newNode;
                return;
            }
            newNode->prev = tail;
            tail->next = newNode;
            tail = newNode;
        }

        // 1. insert node after given node
        void insertAfter(T data, T afterData) {
            Node<T>* newNode = new Node(data);
            if (!head) {
                // empty LL
                head = tail = newNode;
                return;
            }

            // Find after data node
            Node<T>* afterDataNode = head;
            while(afterDataNode && afterDataNode->data != afterData) {
                afterDataNode = afterDataNode->next;
            }

            if (afterDataNode == nullptr) {
                // After data node not found, append at end
                tail->next = newNode;
                newNode->prev = tail;
                tail = newNode;
            } else if(afterDataNode->next) {
                // insert in middle
                newNode->next = afterDataNode->next;
                afterDataNode->next->prev = newNode;
                newNode->prev = afterDataNode;
                afterDataNode->next = newNode;
            } else {
                // insert at end
                newNode->prev = afterDataNode;
                afterDataNode->next = newNode;
                tail = newNode;
            }
        }
        // 2. insert node before given node
        void insertBefore(T data, T beforeData) {
            Node<T>* newNode = new Node(data);
            if (!head) {
                // empty LL
                head = tail = newNode;
                return;
            }

            // Find before data node
            Node<T>* beforeDataNode = head;
            while(beforeDataNode && beforeDataNode->data != beforeData) {
                beforeDataNode = beforeDataNode->next;
            }

            if (beforeDataNode == nullptr) {
                // Before data not found, append at end
                tail->next = newNode;
                newNode->prev = tail;
                tail = newNode;
            } else if(beforeDataNode->prev) {
                // insert in middle
                newNode->prev = beforeDataNode->prev;
                beforeDataNode->prev->next = newNode;
                newNode->next = beforeDataNode;
                beforeDataNode->prev = newNode;
            } else {
                // insert at beginning
                newNode->next = beforeDataNode;
                beforeDataNode->prev = newNode;
                head = newNode;
            }
        }

        // 3. get number of nodes
        int getCount() {
            int count = 0;
            Node<T>* tmp = head;
            while (tmp != nullptr) {
                count++;
                tmp = tmp->next;
            }
            return count;
        }

        // 4. get data in the given
        T getDataByIndex(int index) {
            Node<T>* tmp = head;
            for (int i = 0; i < index; i++) {
                tmp = tmp->next;
            }
            if (tmp) {
                return tmp->data;
            } 
            throw std::out_of_range("Index out of range");
        }

        void removeFirst(T targetData) {
            Node<T> *nodeIt = head;
            while (nodeIt && nodeIt->data != targetData) {
                nodeIt = nodeIt->next;
            }

            if (nodeIt) {
                if (nodeIt->prev) {
                    nodeIt->prev->next = nodeIt->next;
                } else {
                    head = nodeIt->next;
                }
                if (nodeIt->next) {
                    nodeIt->next->prev = nodeIt->prev;
                } else {
                    tail = nodeIt->prev;
                }
                delete nodeIt;
            }
            return;
        }
        // 5. Remove All Nodes
        void removeAll(T targetData) {
            Node<T> *nodeIt = head;

            while (nodeIt) {
                Node<T> *nextNode = nodeIt->next;
                if (nodeIt->data == targetData){
                    removeFirst(targetData);
                }
                nodeIt = nextNode;
            }

        }

        void print() {
            if (!head) {
                std::cout << "[]\n";
                return;
            }
            Node<T> *tmp = head;
            std::cout << "[";
            while (tmp && tmp->next) {
                std::cout << tmp->data << ", ";
                tmp = tmp->next;
            }
            std::cout << tmp->data << "]\n";
        }
};

#endif
