#include <cstdio>
#include <ctime>
#include "stack.h"
#include "unrolled_linked_list.h"
#include "circular_notification_buffer.h"

// Тести частково згенеровані Claude Sonnet 4.6

void test_stack() {
    // Test stack creation
    circle_stack stack = stack_create();
    printf("Stack created. Is empty: %d\n", stack_is_empty(&stack));

    // Test push
    circle c1{1.0, 2.0};
    stack_push(&stack, c1);
    printf("After push: Is empty: %d, Size: %d\n", stack_is_empty(&stack), stack.size);

    // Test peek
    circle peeked = stack_peek(&stack);
    printf("Peeked circle: (%g, %g)\n", peeked.x, peeked.y);

    // Test push another
    circle c2{3.0, 4.0};
    stack_push(&stack, c2);
    printf("After second push: Size: %d\n", stack.size);

    // Test pop
    circle popped = stack_pop(&stack);
    printf("Popped circle: (%g, %g)\n", popped.x, popped.y);
    printf("After pop: Size: %d\n", stack.size);

    // Test pop again
    popped = stack_pop(&stack);
    printf("Popped circle: (%g, %g)\n", popped.x, popped.y);
    printf("After second pop: Is empty: %d, Size: %d\n", stack_is_empty(&stack), stack.size);

    // Test pop from empty
    popped = stack_pop(&stack);
    printf("Pop from empty: (%g, %g)\n", popped.x, popped.y);

    // Test peek from empty
    peeked = stack_peek(&stack);
    printf("Peek from empty: (%g, %g)\n", peeked.x, peeked.y);

    // Test stack_reset
    stack_push(&stack, {5.0, 6.0});
    stack_push(&stack, {7.0, 8.0});
    printf("Before reset: Size: %d\n", stack.size);
    stack_reset(&stack);
    printf("After reset: Is empty: %d, Size: %d\n", stack_is_empty(&stack), stack.size);
    popped = stack_pop(&stack);
    printf("Pop after reset: (%g, %g)\n", popped.x, popped.y);
    peeked = stack_peek(&stack);
    printf("Peek after reset: (%g, %g)\n", peeked.x, peeked.y);
}

bool print_circle(const list_circle c) {
    printf("  (%g, %g)\n", c.x, c.y);
    return true;
}

void test_unrolled_list() {
    unrolled_circle_linked_list list = list_create();

    // Test push_back
    printf("--- push_back 5 elements ---\n");
    list_push_back(&list, {1.0, 1.0});
    list_push_back(&list, {2.0, 2.0});
    list_push_back(&list, {3.0, 3.0});
    list_push_back(&list, {4.0, 4.0});
    list_push_back(&list, {5.0, 5.0});
    printf("Size: %d\n", list.size);

    // Test traverse forward
    printf("--- traverse forward ---\n");
    list_traverse_forward(&list, print_circle);

    // Test traverse backward
    printf("--- traverse backward ---\n");
    list_traverse_backward(&list, print_circle);

    // Test push_front
    printf("--- push_front (0, 0) ---\n");
    list_push_front(&list, {0.0, 0.0});
    printf("Size: %d\n", list.size);
    list_traverse_forward(&list, print_circle);

    // Test insert_at (middle)
    printf("--- insert_at index 3: (99, 99) ---\n");
    list_insert_at(&list, 3, {99.0, 99.0});
    printf("Size: %d\n", list.size);
    list_traverse_forward(&list, print_circle);

    // Test insert_at (beginning)
    printf("--- insert_at index 0: (-1, -1) ---\n");
    list_insert_at(&list, 0, {-1.0, -1.0});
    printf("Size: %d\n", list.size);
    list_traverse_forward(&list, print_circle);

    // Test insert_at (end)
    printf("--- insert_at index %d (end): (100, 100) ---\n", list.size);
    list_insert_at(&list, list.size, {100.0, 100.0});
    printf("Size: %d\n", list.size);
    list_traverse_forward(&list, print_circle);

    // Test element_at
    printf("--- element_at index 0, 3, last ---\n");
    list_circle el = list_element_at(&list, 0);
    printf("  [0]: (%g, %g)\n", el.x, el.y);
    el = list_element_at(&list, 3);
    printf("  [3]: (%g, %g)\n", el.x, el.y);
    el = list_element_at(&list, list.size - 1);
    printf("  [last]: (%g, %g)\n", el.x, el.y);

    // Test element_at out of bounds
    printf("--- element_at out of bounds ---\n");
    el = list_element_at(&list, -1);
    printf("  [-1]: (%g, %g)\n", el.x, el.y);
    el = list_element_at(&list, list.size);
    printf("  [size]: (%g, %g)\n", el.x, el.y);

    // Test search
    printf("--- search for x > 50 ---\n");
    el = list_search_predicate(&list, [](const list_circle c) { return c.x > 50.0; });
    printf("  Found: (%g, %g)\n", el.x, el.y);

    printf("--- search for x == -999 (not found) ---\n");
    el = list_search_predicate(&list, [](const list_circle c) { return c.x == -999.0; });
    printf("  Found: (%g, %g)\n", el.x, el.y);

    // Test remove_at (middle)
    printf("--- remove_at index 3 ---\n");
    list_remove_at(&list, 3);
    printf("Size: %d\n", list.size);
    list_traverse_forward(&list, print_circle);

    // Test remove_at (first)
    printf("--- remove_at index 0 ---\n");
    list_remove_at(&list, 0);
    printf("Size: %d\n", list.size);
    list_traverse_forward(&list, print_circle);

    // Test remove_at (last)
    printf("--- remove_at last ---\n");
    list_remove_at(&list, list.size - 1);
    printf("Size: %d\n", list.size);
    list_traverse_forward(&list, print_circle);

    // Test remove_at out of bounds
    printf("--- remove_at out of bounds (no crash) ---\n");
    list_remove_at(&list, -1);
    list_remove_at(&list, list.size);
    printf("Size unchanged: %d\n", list.size);

    // Test early-exit traversal (stop after x == 3)
    printf("--- traverse forward, stop at x == 3 ---\n");
    list_traverse_forward(&list, [](const list_circle c) {
        printf("  (%g, %g)\n", c.x, c.y);
        return c.x != 3.0;
    });

    // Test clear
    printf("--- clear ---\n");
    list_clear(&list);
    printf("Size after clear: %d\n", list.size);
    list_traverse_forward(&list, print_circle);
}

void print_notification(const notification* n) {
    char time_str[32];
    const tm* t = localtime(&n->timestamp);
    strftime(time_str, sizeof(time_str), "%H:%M:%S", t);
    printf("  [%s] %s\n", time_str, n->message);
}

void print_notifications(const notification** entries, const int count) {
    for (int i = 0; i < count; ++i) {
        print_notification(entries[i]);
    }
}

void test_circular_buffer() {
    const int CAPACITY = 4;

    // Test creation
    circular_notification_buffer buf = buffer_create(CAPACITY);
    printf("Buffer created. Capacity: %d, Is empty: %d\n", buf.capacity, buffer_is_empty(&buf));

    // Test add messages (below capacity)
    printf("--- add 3 messages (below capacity) ---\n");
    buffer_add(&buf, "System started");
    buffer_add(&buf, "User logged in");
    buffer_add(&buf, "File opened");
    printf("Count: %d, Is full: %d\n", buf.count, buffer_is_full(&buf));

    // Test get_last (k < count)
    printf("--- get last 2 ---\n");
    const notification* out[CAPACITY];
    int n = buffer_get_last(&buf, 2, out);
    print_notifications(out, n);

    // Test get_last (k == count)
    printf("--- get last %d (all) ---\n", buf.count);
    n = buffer_get_last(&buf, buf.count, out);
    print_notifications(out, n);

    // Test add up to capacity
    printf("--- add 1 more (fills buffer) ---\n");
    buffer_add(&buf, "Config loaded");
    printf("Count: %d, Is full: %d\n", buf.count, buffer_is_full(&buf));
    n = buffer_get_last(&buf, buf.count, out);
    print_notifications(out, n);

    // Test overwrite oldest (buffer already full)
    printf("--- add 2 more (overwrites oldest entries) ---\n");
    buffer_add(&buf, "Network connected");
    buffer_add(&buf, "Service ready");
    printf("Count: %d\n", buf.count);
    n = buffer_get_last(&buf, buf.count, out);
    print_notifications(out, n);

    // Test get_last k > count (clamped to count)
    printf("--- get last 100 (clamped to count=%d) ---\n", buf.count);
    n = buffer_get_last(&buf, 100, out);
    printf("Returned: %d\n", n);
    print_notifications(out, n);

    // Test get_last k == 0
    printf("--- get last 0 ---\n");
    n = buffer_get_last(&buf, 0, out);
    printf("Returned: %d\n", n);

    // Test get_last k == 1 (most recent)
    printf("--- get last 1 (most recent) ---\n");
    n = buffer_get_last(&buf, 1, out);
    print_notifications(out, n);

    // Test peek_latest and peek_oldest
    printf("--- peek_latest ---\n");
    print_notification(buffer_peek_latest(&buf));
    printf("--- peek_oldest ---\n");
    print_notification(buffer_peek_oldest(&buf));

    // Test traverse (all)
    printf("--- traverse all ---\n");
    buffer_traverse(&buf, [](const notification* entry) {
        print_notification(entry);
        return true;
    });

    // Test traverse with early exit (stop after "Config loaded")
    printf("--- traverse, stop at 'Config loaded' ---\n");
    buffer_traverse(&buf, [](const notification* entry) {
        print_notification(entry);
        return strcmp(entry->message, "Config loaded") != 0;
    });

    // Test empty buffer peek
    printf("--- peek on empty buffer ---\n");
    circular_notification_buffer empty = buffer_create(CAPACITY);
    printf("peek_latest is null: %d\n", buffer_peek_latest(&empty) == nullptr);
    printf("peek_oldest is null: %d\n", buffer_peek_oldest(&empty) == nullptr);
    n = buffer_get_last(&empty, 3, out);
    printf("get_last from empty returned: %d\n", n);
    buffer_destroy(&empty);

    // Test destroy
    printf("--- destroy ---\n");
    buffer_destroy(&buf);
    printf("After destroy: head is null: %d, tail is null: %d\n", buf.head == nullptr, buf.tail == nullptr);
}

int main()
{
    printf("Testing stack implementation\n");
    test_stack();

    printf("\n========================\n\n");

    printf("Testing unrolled linked list implementation\n");
    test_unrolled_list();

    printf("\n========================\n\n");

    printf("Testing circular notification buffer\n");
    test_circular_buffer();

    printf("\n========================\n\n");

    return 0;
}