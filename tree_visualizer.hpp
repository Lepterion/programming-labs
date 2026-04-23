//
// Created by bopet on 19/04/2026.
//

#ifndef TREE_VISUALIZER_HPP
#define TREE_VISUALIZER_HPP

#include <iostream>
#include <vector>
#include <string>
#include "common_tree.h"
#include "binary_tree.h"
#include "expression_tree.h"

namespace Visualizer {

    struct DrawNode {
        std::string text;
        DrawNode* left = nullptr;
        DrawNode* right = nullptr;
        std::vector<DrawNode*> children; // Для загального дерева
        int x = 0;
        int y = 0;
    };

    inline void freeTree(const DrawNode* node) {
        if (!node) return;

        freeTree(node->left);
        freeTree(node->right);

        for (auto* c : node->children) freeTree(c);

        delete node;
    }

    inline DrawNode* buildNode(const CommonNode* root) {
        if (!root) return nullptr;

        auto n = new DrawNode();
        n->text = std::to_string(root->data);

        for (auto* c : root->children) {
            n->children.push_back(buildNode(c));
        }

        return n;
    }

    inline DrawNode* buildNode(const BinNode* root) {
        if (!root) return nullptr;

        auto n = new DrawNode();
        n->text = std::to_string(root->data) + (root->isThreaded ? "(T)" : "");
        n->left = buildNode(root->left);

        if (!root->isThreaded) {
            n->right = buildNode(root->right);
        }

        return n;
    }

    inline DrawNode* buildNode(const ExprNode* root) {
        if (!root) return nullptr;

        auto n = new DrawNode();
        n->text = root->value;
        n->left = buildNode(root->left);
        n->right = buildNode(root->right);

        return n;
    }

    inline void layoutBinary(DrawNode* node, const int depth, int& current_x) {
        if (!node) return;

        layoutBinary(node->left, depth + 1, current_x);

        int default_x = current_x;
        current_x += node->text.length() + 2; // +2 для відступів між вузлами

        layoutBinary(node->right, depth + 1, current_x);

        if (node->left && node->right) {
            int left_center = node->left->x + node->left->text.length() / 2;
            int right_center = node->right->x + node->right->text.length() / 2;
            node->x = (left_center + right_center) / 2 - node->text.length() / 2;
        } else {
            node->x = default_x;
        }

        node->y = depth;
    }

    inline void layoutCommon(DrawNode* node, const int depth, int& current_x) {
        if (!node) return;

        if (node->children.empty()) {
            node->x = current_x;
            node->y = depth;
            current_x += node->text.length() + 2;
            return;
        }

        for (auto* c : node->children) {
            layoutCommon(c, depth + 1, current_x);
        }

        // Центруємо батька між першою і останньою дитиною
        const int first_x = node->children.front()->x;
        const int last_x = node->children.back()->x;
        node->x = (first_x + last_x) / 2;
        node->y = depth;
    }

    inline void renderDrawNode(DrawNode* root, const bool isBinary) {
        if (!root) return;

        int max_x = 0;
        int max_y = 0;

        auto findBounds = [&](DrawNode* n, auto& self) -> void {
            if (!n) return;
            if (n->x + (int)n->text.length() > max_x) max_x = n->x + n->text.length();
            if (n->y > max_y) max_y = n->y;
            self(n->left, self);
            self(n->right, self);
            for (auto* c : n->children) self(c, self);
        };

        findBounds(root, findBounds);

        std::vector<std::string> lines((max_y + 1) * 2, std::string(max_x + 2, ' '));

        auto draw = [&](DrawNode* n, auto& self) -> void {
            if (!n) return;

            // Малюємо сам текст
            for (size_t i = 0; i < n->text.length(); ++i) {
                lines[n->y * 2][n->x + i] = n->text[i];
            }

            const int parent_center = n->x + n->text.length() / 2;

            if (isBinary) {
                if (n->left) {
                    const int child_center = n->left->x + n->left->text.length() / 2;
                    for (int x = child_center + 1; x < parent_center; ++x) lines[n->y * 2 + 1][x] = '_';

                    lines[n->y * 2 + 1][child_center] = '/';
                    self(n->left, self);
                }

                if (n->right) {
                    const int child_center = n->right->x + n->right->text.length() / 2;
                    for (int x = parent_center + 1; x < child_center; ++x) lines[n->y * 2 + 1][x] = '_';

                    lines[n->y * 2 + 1][child_center] = '\\';
                    self(n->right, self);
                }
            } else {
                if (!n->children.empty()) {
                    for (auto* c : n->children) {
                        int child_center = c->x + c->text.length() / 2;
                        lines[n->y * 2 + 1][child_center] = '|';
                        self(c, self);
                    }

                    const int first_c = n->children.front()->x + n->children.front()->text.length() / 2;
                    const int last_c = n->children.back()->x + n->children.back()->text.length() / 2;

                    for (int x = first_c; x <= last_c; ++x) {
                        if (lines[n->y * 2 + 1][x] == ' ') lines[n->y * 2 + 1][x] = '-';
                    }

                    lines[n->y * 2 + 1][parent_center] = '+';
                }
            }
        };

        draw(root, draw);

        for (const auto& line : lines) {
            std::string trimmed = line;
            trimmed.erase(trimmed.find_last_not_of(" \n\r\t") + 1);
            if (!trimmed.empty()) {
                std::cout << trimmed << "\n";
            }
        }
    }

    inline void printTree(const CommonNode* root) {
        DrawNode* d = buildNode(root);
        int current_x = 0;
        layoutCommon(d, 0, current_x);
        renderDrawNode(d, false);
        freeTree(d);
    }

    inline void printTree(const BinNode* root) {
        DrawNode* d = buildNode(root);
        int current_x = 0;
        layoutBinary(d, 0, current_x);
        renderDrawNode(d, true);
        freeTree(d);
    }

    inline void printTree(const ExprNode* root) {
        DrawNode* d = buildNode(root);
        int current_x = 0;
        layoutBinary(d, 0, current_x);
        renderDrawNode(d, true);
        freeTree(d);
    }
}

#endif // TREE_VISUALIZER_HPP