//
// Created by bopet on 16/04/2026.
//

#include "common_tree.h"
#include <iostream>

// повертає true, якщо додавання пройшло успішно, false - якщо шлях не існує
bool CommonTree::addByPath(int val, const std::vector<int>& path) {
    if (!root) {
        if (path.empty()) {
            root = new CommonNode(val);
            return true;
        }
        return false;
    }

    CommonNode* current = root;
    for (int index : path) {
        if (index < 0 || index >= current->children.size()) {
            return false; // Шлях не існує
        }
        current = current->children[index];
    }

    // Пам'ять має видалити консумер
    current->children.push_back(new CommonNode(val));
    return true;
}

void CommonTree::print(CommonNode* node, int depth) {
    if (!node) return;

    for (int i = 0; i < depth; ++i) std::cout << "  ";

    std::cout << "- " << node->data << "\n";

    for (CommonNode* child : node->children) {
        print(child, depth + 1);
    }
}

CommonNode* CommonTree::deleteByValue(int val) {
    if (!root) return nullptr;

    if (root->data == val) {
        CommonNode* temp = root;
        root = nullptr;
        return temp;
    }

    return deleteHelper(root, val);
}

CommonNode* CommonTree::deleteHelper(CommonNode* current, int val) {
    for (auto it = current->children.begin(); it != current->children.end(); ++it) {
        if ((*it)->data == val) {
            CommonNode* deletedSubtree = *it;
            current->children.erase(it);
            return deletedSubtree;
        }

        if (CommonNode* found = deleteHelper(*it, val)) return found;
    }
    return nullptr;
}