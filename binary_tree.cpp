//
// Created by bopet on 16/04/2026.
//

#include "binary_tree.h"
#include <iostream>

void BinaryTree::add(const int val) {
    addHelper(root, val);
}

void BinaryTree::addHelper(BinNode*& node, int val) {
    if (!node) {
        node = new BinNode(val);
        return;
    }

    // Якщо дерево вже прошите, то додавання зламає прошивку
    if (val < node->data) {
        addHelper(node->left, val);
    } else {
        if (node->isThreaded) {
            BinNode* temp = node->right;
            node->right = new BinNode(val);
            node->isThreaded = false;
            node->right->right = temp;
            node->right->isThreaded = true;
        } else {
            addHelper(node->right, val);
        }
    }
}

void BinaryTree::print(const BinNode* node, const int depth) {
    if (!node) return;

    print(node->left, depth + 1);
    for (int i = 0; i < depth; ++i) std::cout << "  ";
    std::cout << "- " << node->data << (node->isThreaded ? " (T)" : "") << "\n";

    if (!node->isThreaded) {
        print(node->right, depth + 1);
    }
}

void BinaryTree::createThreaded() {
    createThreadedHelper(root, nullptr);
}

BinNode* BinaryTree::createThreadedHelper(BinNode* node, BinNode* prev) {
    if (!node) return prev;

    BinNode* leftRes = createThreadedHelper(node->left, prev);

    if (leftRes && !leftRes->right) {
        leftRes->right = node;
        leftRes->isThreaded = true;
    }

    return createThreadedHelper(node->right, node);
}