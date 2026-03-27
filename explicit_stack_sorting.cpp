//
// Created by bopet on 19/03/2026.
//

#include "explicit_stack_sorting.h"
#include "sorts.h"
#include <stack>
#include <utility>

void quickSortIterative(std::vector<std::string>& arr, int low, int high, bool demo, std::ostream& out) {
    std::stack<std::pair<int, int>> stk;
    stk.push({low, high});

    while (!stk.empty()) {
        int curr_low = stk.top().first;
        int curr_high = stk.top().second;
        stk.pop();

        if (curr_low < curr_high) {
            int pi = hoarePartition(arr, curr_low, curr_high, demo, out);

            int left_size = pi - curr_low;
            int right_size = curr_high - (pi + 1);

            if (left_size > right_size) {
                if (curr_low < pi) stk.push({curr_low, pi});
                if (pi + 1 < curr_high) stk.push({pi + 1, curr_high});
            } else {
                if (pi + 1 < curr_high) stk.push({pi + 1, curr_high});
                if (curr_low < pi) stk.push({curr_low, pi});
            }
        }
    }
}

void combinedSortIterative(std::vector<std::string>& arr, int low, int high, int threshold) {
    std::stack<std::pair<int, int>> stk;
    stk.push({low, high});

    while (!stk.empty()) {
        int curr_low = stk.top().first;
        int curr_high = stk.top().second;
        stk.pop();

        if (curr_high - curr_low + 1 <= threshold) {
            insertionSort(arr, curr_low, curr_high, false, std::cout);
        } else if (curr_low < curr_high) {
            int pi = hoarePartition(arr, curr_low, curr_high, false, std::cout);

            int left_size = pi - curr_low;
            int right_size = curr_high - (pi + 1);

            if (left_size > right_size) {
                if (curr_low < pi) stk.push({curr_low, pi});
                if (pi + 1 < curr_high) stk.push({pi + 1, curr_high});
            } else {
                if (pi + 1 < curr_high) stk.push({pi + 1, curr_high});
                if (curr_low < pi) stk.push({curr_low, pi});
            }
        }
    }
}