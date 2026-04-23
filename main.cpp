/*
 * Лабораторна робота №4
 * Варіант: 7 (шлях розраховано за алгоритмом номер_варіанту mod кількість_завдань)
 * Блок 0: Дерево з довільною кількістю дітей
 * Блок 1, завдання 1: Додавання за шляхом
 * Блок 2, завдання 2: Вивід з відступами
 * Блок 3, завдання 2: Видалення за значенням (повертає піддерево)
 * Блок 4, завдання 1: Бінарне дерево, додавання за значенням
 * Блок 5, завдання 1: Прошите дерево
 * Блок 6, завдання 2: Дерево виразів (обчислення, спрощення, похідна)
 * (номери завдань відносно блоків)
 */

#include <iostream>
#include "common_tree.h"
#include "binary_tree.h"
#include "expression_tree.h"
#include "tree_visualizer.hpp"

void runDemo(const bool useVerticalPrint) {
    std::cout << "\n=== DEMO MODE ===\n";

    std::cout << "\n1. General Tree (Blocks 0-3)\n";
    CommonTree ct;
    ct.addByPath(10, {});
    ct.addByPath(20, {});
    ct.addByPath(30, {});
    ct.addByPath(40, {0});

    if (useVerticalPrint) Visualizer::printTree(ct.root);
    else CommonTree::print(ct.root);

    // -----------------------------------------------------------

    std::cout << "\nDeleting subtree with value 20:\n";
    const CommonNode* deleted = ct.deleteByValue(20);
    if (useVerticalPrint) Visualizer::printTree(ct.root);
    else CommonTree::print(ct.root);
    delete deleted;

    // -----------------------------------------------------------

    std::cout << "\n2. Threaded Binary Tree (Blocks 4-5)\n";
    BinaryTree bt;
    bt.add(50);
    bt.add(30);
    bt.add(70);
    bt.add(20);
    bt.add(40);
    bt.createThreaded();

    if (useVerticalPrint) Visualizer::printTree(bt.root);
    else BinaryTree::print(bt.root);

    // -----------------------------------------------------------

    std::cout << "\n3. Expression Tree: 5 * x + 3 (Block 6)\n";
    ExpressionTree et;
    et.root = new ExprNode("OP", "+");
    et.root->left = new ExprNode("OP", "*");
    et.root->left->left = new ExprNode("CONST", "5");
    et.root->left->right = new ExprNode("VAR", "x");
    et.root->right = new ExprNode("CONST", "3");

    if (useVerticalPrint) Visualizer::printTree(et.root);
    else ExpressionTree::print(et.root);

    std::cout << "\nEvaluation at x = 2: " << ExpressionTree::evaluate(et.root, {{"x", 2.0}}) << "\n";

    std::cout << "\nDerivative with respect to x (before simplification):\n";
    ExprNode* diffNode = ExpressionTree::differentiate(et.root, "x");
    if (useVerticalPrint) Visualizer::printTree(diffNode);
    else ExpressionTree::print(diffNode);

    std::cout << "\nDerivative with respect to x (after simplification):\n";
    ExpressionTree::simplify(diffNode);
    if (useVerticalPrint) Visualizer::printTree(diffNode);
    else ExpressionTree::print(diffNode);
}

void runInteractive(const bool useVerticalPrint) {
    std::cout << "\n=== INTERACTIVE MODE ===\n";
    BinaryTree bt;
    int choice;
    do {
        std::cout << "1. Add to binary tree\n2. Print tree\n3. Thread the tree\n0. Exit\nChoice: ";
        std::cin >> choice;
        if (choice == 1) {
            int val;
            std::cout << "Value: ";
            std::cin >> val;
            bt.add(val);
        } else if (choice == 2) {
            if (useVerticalPrint) {
                Visualizer::printTree(bt.root);
            } else {
                BinaryTree::print(bt.root);
            }
        } else if (choice == 3) {
            bt.createThreaded();
            std::cout << "Tree has been threaded.\n";
        }
    } while (choice != 0);
}

int main() {
    int printStyle;
    std::cout << "Choose print style:\n";
    std::cout << "1 - Horizontal (indents, standard)\n";
    std::cout << "2 - Vertical (graphical visualization)\nChoice: ";
    std::cin >> printStyle;
    const bool useVertical = printStyle == 2;

    int mode;
    std::cout << "\nChoose mode (1 - Demo, 2 - Interactive): ";
    std::cin >> mode;

    if (mode == 1) {
        runDemo(useVertical);
    } else if (mode == 2) {
        runInteractive(useVertical);
    } else {
        std::cout << "Unknown mode.\n";
    }

    return 0;
}