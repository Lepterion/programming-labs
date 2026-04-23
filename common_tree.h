//
// Created by bopet on 16/04/2026.
//

#ifndef PROGRAMMING_LABS_COMMON_TREE_H
#define PROGRAMMING_LABS_COMMON_TREE_H

#include <vector>

struct CommonNode {
    int data;
    std::vector<CommonNode*> children;
    CommonNode(int val) : data(val) {}
};

class CommonTree {
public:
    CommonNode* root;

    CommonTree() : root(nullptr) {}

    // Шлях - це послідовність індексів дітей. Порожній шлях - це корінь.
    bool addByPath(int val, const std::vector<int>& path);

    CommonNode* deleteByValue(int val);

    static void print(CommonNode* node, int depth = 0);

private:
    static CommonNode* deleteHelper(CommonNode* current, int val);
};

#endif //PROGRAMMING_LABS_COMMON_TREE_H