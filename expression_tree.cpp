//
// Created by bopet on 16/04/2026.
//

#include "expression_tree.h"

void ExpressionTree::print(ExprNode* node, int depth) {
    if (!node) return;

    print(node->right, depth + 1);

    for (int i = 0; i < depth; ++i) std::cout << "  ";

    std::cout << node->value << "\n";

    print(node->left, depth + 1);
}

double ExpressionTree::evaluate(ExprNode* node, const std::map<std::string, double>& vars) {
    if (!node) return 0;

    if (node->type == "CONST") return std::stod(node->value);
    if (node->type == "VAR") return vars.at(node->value);

    double leftVal = evaluate(node->left, vars);
    double rightVal = evaluate(node->right, vars);

    if (node->value == "+") return leftVal + rightVal;
    if (node->value == "*") return leftVal * rightVal;
    if (node->value == "/") {
        if (rightVal == 0) throw std::runtime_error("Division by zero!");
        return leftVal / rightVal;
    }

    return 0;
}

ExprNode* ExpressionTree::differentiate(ExprNode* node, const std::string& var) {
    if (!node) return nullptr;

    if (node->type == "CONST") return new ExprNode("CONST", "0");
    if (node->type == "VAR") return new ExprNode("CONST", node->value == var ? "1" : "0");

    if (node->value == "+") {
        auto res = new ExprNode("OP", "+");
        res->left = differentiate(node->left, var);
        res->right = differentiate(node->right, var);
        return res;
    }

    if (node->value == "*") {
        // (u*v)' = u'*v + u*v'
        auto res = new ExprNode("OP", "+");

        auto leftPart = new ExprNode("OP", "*");
        leftPart->left = differentiate(node->left, var);
        leftPart->right = new ExprNode(node->right->type, node->right->value); // клонування праворуч

        auto* rightPart = new ExprNode("OP", "*");
        rightPart->left = new ExprNode(node->left->type, node->left->value); // клонування ліворуч
        rightPart->right = differentiate(node->right, var);

        res->left = leftPart;
        res->right = rightPart;

        return res;
    }

    return nullptr;
}

void ExpressionTree::simplify(ExprNode*& node) {
    if (!node) return;

    simplify(node->left);
    simplify(node->right);

    if (node->type == "OP") {
        // Спрощення констант
        if (node->left && node->left->type == "CONST" && node->right && node->right->type == "CONST") {
            double res = evaluate(node, {});
            node->type = "CONST";

            // Зберігаємо цілі числа без нулів після коми
            node->value = std::to_string((int)res);
            delete node->left; delete node->right;
            node->left = node->right = nullptr;
        }
    }
}