//
// Created by bopet on 16/04/2026.
//

#ifndef PROGRAMMING_LABS_EXPRESSION_TREE_H
#define PROGRAMMING_LABS_EXPRESSION_TREE_H

#include <string>
#include <map>
#include <iostream>

// Типи вузлів: CONST, VAR, OP (+, *)
struct ExprNode {
    std::string type;
    std::string value;
    ExprNode* left;
    ExprNode* right;

    ExprNode(std::string t, std::string v) : type(t), value(v), left(nullptr), right(nullptr) {}
};

class ExpressionTree {
public:
    ExprNode* root;
    ExpressionTree() : root(nullptr) {}

    static void print(ExprNode* node, int depth = 0);

    static double evaluate(ExprNode* node, const std::map<std::string, double>& vars);

    static ExprNode* differentiate(ExprNode* node, const std::string& var);

    static void simplify(ExprNode*& node);
};

#endif //PROGRAMMING_LABS_EXPRESSION_TREE_H