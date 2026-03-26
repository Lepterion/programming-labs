//
// Created by bopet on 19/03/2026.
//

#ifndef EXPLICIT_STACK_SORTING_H
#define EXPLICIT_STACK_SORTING_H

#include <vector>
#include <string>
#include <ostream>
#include <iostream>

// Ітеративний Quicksort (явний стек)
void quickSortIterative(std::vector<std::string>& arr, int low, int high, bool demo = false, std::ostream& out = std::cout);

// Ітеративний Комбінований алгоритм (явний стек)
void combinedSortIterative(std::vector<std::string>& arr, int low, int high, int threshold);

#endif // EXPLICIT_STACK_SORTING_H