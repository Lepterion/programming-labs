//
// Created by bopet on 22/02/2026.
//

#ifndef PROGRAMMING_LABS_STACK_H
#define PROGRAMMING_LABS_STACK_H

// Варіант №2, підваріант №4

struct circle {
    double x;
    double y;
};

struct stack_node {
    circle circle;
    stack_node *next;
};

struct circle_stack {
    stack_node *head;
    int size;
};

inline circle_stack stack_create() {
    return {};
}

inline bool stack_is_empty(const circle_stack* stack) {
    return stack->size == 0;
}

inline void stack_push(circle_stack* stack, const circle circle) {
    stack->size++;

    auto* new_node = new stack_node;
    new_node->circle = circle;
    new_node->next = stack->head;

    stack->head = new_node;
}

inline circle stack_pop(circle_stack* stack) {
    if (stack->size == 0) {
        return {};
    }

    const circle circle = stack->head->circle;

    const auto currentHead = stack->head;
    stack->head = currentHead->next;

    delete currentHead;

    stack->size--;

    return circle;
}

inline circle stack_peek(const circle_stack* stack) {
    if (stack->size == 0) {
        return {};
    }

    return stack->head->circle;
}

inline void stack_reset(circle_stack* stack) {
    while (stack->head != nullptr) {
        const auto currentHead = stack->head;
        stack->head = currentHead->next;

        delete currentHead;
    }

    stack->size = 0;
    stack->head = nullptr;
}

#endif //PROGRAMMING_LABS_STACK_H