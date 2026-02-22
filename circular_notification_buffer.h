//
// Created by bopet on 22/02/2026.
//

#ifndef PROGRAMMING_LABS_CIRCULAR_NOTIFICATION_BUFFER_H
#define PROGRAMMING_LABS_CIRCULAR_NOTIFICATION_BUFFER_H

// Завдання ** 16

#include <ctime>
#include <cstring>

struct notification {
    char* message;   // heap-allocated, null-terminated
    time_t timestamp;
};

struct notification_node {
    notification entry;
    notification_node* prev;
    notification_node* next;
};

struct circular_notification_buffer {
    notification_node* head; // oldest entry
    notification_node* tail; // newest entry
    int capacity;
    int count;
};

inline void notification_free(notification* n) {
    delete[] n->message;
    n->message = nullptr;
}

inline circular_notification_buffer buffer_create(const int capacity) {
    circular_notification_buffer buf{};
    buf.capacity = capacity;
    return buf;
}

inline void buffer_destroy(circular_notification_buffer* buf) {
    notification_node* current = buf->head;
    while (current != nullptr) {
        notification_node* next = current->next;
        notification_free(&current->entry);
        delete current;
        current = next;
    }

    buf->head = nullptr;
    buf->tail = nullptr;
    buf->capacity = 0;
    buf->count = 0;
}

inline void buffer_add(circular_notification_buffer* buf, const char* message) {
    notification_node* node;

    if (buf->count < buf->capacity) {
        node = new notification_node{};
        node->prev = buf->tail;
        node->next = nullptr;

        if (buf->tail != nullptr) {
            buf->tail->next = node;
        } else {
            buf->head = node;
        }

        buf->tail = node;
        buf->count++;
    } else {
        node = buf->head;

        if (buf->head->next != nullptr) {
            buf->head->next->prev = nullptr;
        }

        buf->head = buf->head->next;

        node->prev = buf->tail;
        node->next = nullptr;
        buf->tail->next = node;
        buf->tail = node;

        notification_free(&node->entry);
    }

    const size_t len = strlen(message);
    node->entry.message = new char[len + 1];
    memcpy(node->entry.message, message, len + 1);
    node->entry.timestamp = time(nullptr);
}

inline int buffer_get_last(const circular_notification_buffer* buf, int k, const notification** out) {
    if (k > buf->count) k = buf->count;
    if (k <= 0) return 0;

    const notification_node* current = buf->head;
    for (int skip = buf->count - k; skip > 0; --skip) {
        current = current->next;
    }

    for (int i = 0; i < k; ++i) {
        out[i] = &current->entry;
        current = current->next;
    }

    return k;
}

inline const notification* buffer_peek_latest(const circular_notification_buffer* buf) {
    if (buf->tail == nullptr) return nullptr;
    return &buf->tail->entry;
}

inline const notification* buffer_peek_oldest(const circular_notification_buffer* buf) {
    if (buf->head == nullptr) return nullptr;
    return &buf->head->entry;
}

inline void buffer_traverse(const circular_notification_buffer* buf, bool (*callback)(const notification*)) {
    const notification_node* current = buf->head;
    while (current != nullptr) {
        if (!callback(&current->entry)) return;
        current = current->next;
    }
}

inline bool buffer_is_empty(const circular_notification_buffer* buf) {
    return buf->count == 0;
}

inline bool buffer_is_full(const circular_notification_buffer* buf) {
    return buf->count == buf->capacity;
}

#endif //PROGRAMMING_LABS_CIRCULAR_NOTIFICATION_BUFFER_H

