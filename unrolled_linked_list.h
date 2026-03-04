//
// Created by bopet on 22/02/2026.
//

#ifndef PROGRAMMING_LABS_UNROLLED_LINKED_LIST_H
#define PROGRAMMING_LABS_UNROLLED_LINKED_LIST_H

// Завдання *** 26

#define ARRAY_SIZE 4

struct list_circle {
    double x;
    double y;
};

struct list_node {
    list_circle circles[ARRAY_SIZE];
    int count;
    list_node* next;
    list_node* prev;
};

struct unrolled_circle_linked_list {
    list_node* head;
    list_node* tail;
    int size;
};

inline unrolled_circle_linked_list list_create() {
    return {};
}

inline void list_push_back(unrolled_circle_linked_list* list, const list_circle circle) {
    if (list->tail == nullptr || list->tail->count == ARRAY_SIZE) {
        auto* new_node = new list_node;
        new_node->count = 0;
        new_node->next = nullptr;
        new_node->prev = list->tail;

        if (list->tail != nullptr) {
            list->tail->next = new_node;
        }

        list->tail = new_node;

        if (list->head == nullptr) {
            list->head = new_node;
        }
    }

    list->tail->circles[list->tail->count++] = circle;
    list->size++;
}

inline void list_push_front(unrolled_circle_linked_list* list, const list_circle circle) {
    if (list->head == nullptr || list->head->count == ARRAY_SIZE) {
        auto* new_node = new list_node;
        new_node->count = 0;
        new_node->next = list->head;
        new_node->prev = nullptr;

        if (list->head != nullptr) {
            list->head->prev = new_node;
        }

        list->head = new_node;

        if (list->tail == nullptr) {
            list->tail = new_node;
        }
    }

    for (int i = list->head->count; i > 0; --i) {
        list->head->circles[i] = list->head->circles[i - 1];
    }
    
    list->head->circles[0] = circle;
    list->head->count++;
    list->size++;
}

inline void list_insert_at(unrolled_circle_linked_list* list, int index, const list_circle circle) {
    if (index < 0 || index > list->size) {
        return;
    }

    if (index == 0) {
        list_push_front(list, circle);
        return;
    }

    if (index == list->size) {
        list_push_back(list, circle);
        return;
    }

    list_node* current = list->head;
    while (current != nullptr) {
        if (index <= current->count) {
            // If node is full, split it first
            if (current->count == ARRAY_SIZE) {
                auto* new_node = new list_node;
                const int half = ARRAY_SIZE / 2;
                new_node->count = current->count - half;
                for (int i = 0; i < new_node->count; ++i) {
                    new_node->circles[i] = current->circles[half + i];
                }
                current->count = half;

                new_node->next = current->next;
                new_node->prev = current;
                if (current->next != nullptr) {
                    current->next->prev = new_node;
                } else {
                    list->tail = new_node;
                }
                current->next = new_node;

                // Decide which node to insert into after split
                if (index > half) {
                    index -= half;
                    current = new_node;
                }
            }

            for (int i = current->count; i > index; --i) {
                current->circles[i] = current->circles[i - 1];
            }

            current->circles[index] = circle;
            current->count++;
            list->size++;

            return;
        }

        index -= current->count;
        current = current->next;
    }
}

inline list_circle list_element_at(const unrolled_circle_linked_list* list, int index) {
    if (index < 0 || index >= list->size) {
        return {};
    }

    const list_node* current = list->head;
    while (current != nullptr) {
        if (index < current->count) {
            return current->circles[index];
        }

        index -= current->count;
        current = current->next;
    }

    return {};
}

inline void list_remove_at(unrolled_circle_linked_list* list, int index) {
    if (index < 0 || index >= list->size) {
        return;
    }

    list_node* current = list->head;
    while (current != nullptr) {
        if (index < current->count) {
            for (int i = index; i < current->count - 1; ++i) {
                current->circles[i] = current->circles[i + 1];
            }

            current->count--;
            list->size--;

            if (current->count == 0) {
                if (current->prev != nullptr) {
                    current->prev->next = current->next;
                } else {
                    list->head = current->next;
                }

                if (current->next != nullptr) {
                    current->next->prev = current->prev;
                } else {
                    list->tail = current->prev;
                }

                delete current;
            }

            return;
        }

        index -= current->count;
        current = current->next;
    }
}

inline void list_clear(unrolled_circle_linked_list* list) {
    const list_node* current = list->head;
    while (current != nullptr) {
        const list_node* next = current->next;
        delete current;
        current = next;
    }

    list->head = nullptr;
    list->tail = nullptr;
    list->size = 0;
}

inline list_circle list_search_predicate(const unrolled_circle_linked_list* list, bool (*predicate)(const list_circle)) {
    const list_node* current = list->head;
    while (current != nullptr) {
        for (int i = 0; i < current->count; ++i) {
            if (predicate(current->circles[i])) {
                return current->circles[i];
            }
        }

        current = current->next;
    }

    return {};
}

inline void list_traverse_forward(const unrolled_circle_linked_list* list, bool (*callback)(const list_circle)) {
    const list_node* current = list->head;
    while (current != nullptr) {
        for (int i = 0; i < current->count; ++i) {
            if (!callback(current->circles[i])) return;
        }

        current = current->next;
    }
}

inline void list_traverse_backward(const unrolled_circle_linked_list* list, bool (*callback)(const list_circle)) {
    const list_node* current = list->tail;
    while (current != nullptr) {
        for (int i = current->count - 1; i >= 0; --i) {
            if (!callback(current->circles[i])) return;
        }

        current = current->prev;
    }
}

#endif //PROGRAMMING_LABS_UNROLLED_LINKED_LIST_H