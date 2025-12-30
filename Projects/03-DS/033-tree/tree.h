#ifndef TREE_H
#define TREE_H

#include <iostream>
#include "node.h"

class Tree{
    Node*  root;
    
    Node* getNodeByData(int data) {
        Node* current = root;
        while (current != nullptr) {
            if (data == current->data) {
                return current ;
            } else if (data > current->data) {
                current = current->right;
            } else {
                current = current->left;
            }
        }
        return nullptr;
    }
    
    Node* getParent(Node* node) {
        if (node == root) { return nullptr; }
        if (node) {
            Node* parent = root;
            while (parent) {
                if (parent->left == node || parent->right == node) {
                    return parent;
                } else if (node->data > parent->data){
                    parent = parent->right;
                } else {
                    parent = parent->left;
                }
            }
        }
        return nullptr;
    }

    Node* getMaxRight(Node* node) {
        if (!node) { return nullptr; }
        while (node->right) { node = node->right; }
        return node;
    }

    void displayLDR(Node* node) {
        // In order traversal
        if (!node) { return; } // Base Case
        displayLDR(node->left);
        std::cout << node->data << "  ";
        displayLDR(node->right);
    }

    int getMaxDepthRec(Node* node, int currHeight) {
        if (!node) return currHeight;
        if (node->left == nullptr && node->right == nullptr) return currHeight + 1;
        int left = 0;
        int right = 0;
        if (node->left) {
            left = getMaxDepthRec(node->left, currHeight + 1);
        }
        if (node->right) {
            right = getMaxDepthRec(node->right, currHeight + 1);
        }
        return right > left ? right : left;
    }

public :
    Tree() { root=nullptr; }

    void add(int data) {
        Node* newNode = new Node(data);
        
        // Build Connections
        if (root == nullptr) { // Empty Tree
            root = newNode;
        } else { // Non-Empty Tree
            Node* current = root;
            Node* parent = nullptr;
            while (current != nullptr) {
                parent = current;
                if (data > current->data) {
                    current = current->right;
                } else {
                    current = current->left;
                }
            }
            if (data > parent->data) {
                parent->right = newNode;
            } else {
                parent->left = newNode;
            }
        }
    }
    
    bool checkDataInTree(int data){
        return getNodeByData(data) != nullptr;
    }

    int getParentData(int data){
        Node* node = getNodeByData(data);
        if (node) {
            Node* parent = getParent(node);
            if (parent) {
                return parent->data;
            } else {
                return -100; // Root Node
            }
        } else {
            return -404; //NotFound
        }
    }

    int getMaxRightData(int data){
        Node* node = getNodeByData(data);
        if (node) {
            Node* maxRight = getMaxRight(node);
            return maxRight->data;
        }
        return -404; // Not Found
    }
    
    void removeNode(int data) {
        Node* current=getNodeByData(data);
        if (!current) { return; }
        if (current == root) {
            if (root->left == nullptr && root->right == nullptr) { // ROOT Only
                root = nullptr;
            } else if (root->right == nullptr) { // ROOT & Left Side
                root = root->left;
            } else if (root->left == nullptr) { // ROOT & RightSide
                root = root->right;
            } else { // ROOT & Right & Left Side
                Node* newRoot = root->left;
                Node* maxRight = getMaxRight(newRoot);
                maxRight->right = root->right;
                root = newRoot;
            }
        } else { // Node Not Root
            Node* parent = getParent(current);
            Node* child = nullptr;
            // leaf Node don't Have any Child
            if (current->right == nullptr && current->left == nullptr) {
                child = nullptr;
            } else if (current->right == nullptr) { // Left Child Only
                child = current->left;
            } else if (current->left == nullptr) { // Right Child Only
                child = current->right;
            } else { // Left & Right Child
                Node* newparent = current->left;
                Node* maxRight = getMaxRight(newparent);
                maxRight->right = current->right;
                child = newparent;
            }
            if (current->data > parent->data) {
                parent->right = child;
            } else {
                parent->left = child;
            }
        }
        delete current ;
    }
    
    void traverse() { displayLDR(root); }

    int getMaxDepth() {
        return getMaxDepthRec(root, 0);
        // if (!root) return 0;
        // int currCount = 0;
        // int maxCount = 0;
        // Node *current = root;
        // Node* parent = nullptr;
        // Node* grand = nullptr;
        // while (current) {
        //     currCount++;
        //     if (current->left && current-> right) {
        //         grand = parent;
        //         parent = current;
        //         current = current->left;
        //     }
        //     if (current->left) {
        //         grand = parent;
        //         parent = current;
        //         current = current->left;
        //     } else if (current->right) {
        //         grand = parent;
        //         parent = current;
        //         current = current->left;
        //     } else {
        //         if (currCount > maxCount) maxCount = currCount;
        //         currCount--;
        //         current = parent;
        //         parent = grand;
        //     }
        // }
    }
};

#endif