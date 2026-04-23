//
// Created by bopet on 16/04/2026.
//

#ifndef PROGRAMMING_LABS_BINARY_TREE_H
#define PROGRAMMING_LABS_BINARY_TREE_H

struct BinNode {
    int data;
    BinNode* left;
    BinNode* right;
    bool isThreaded; // Для блоку №5
    BinNode(int val) : data(val), left(nullptr), right(nullptr), isThreaded(false) {}
};

class BinaryTree {
public:
    BinNode* root;

    BinaryTree() : root(nullptr) {}

    void add(int val);

    static void print(const BinNode *node, int depth = 0);

    void createThreaded();

private:
    static void addHelper(BinNode*& node, int val);

    static BinNode* createThreadedHelper(BinNode* node, BinNode* prev);
};

#endif //PROGRAMMING_LABS_BINARY_TREE_H