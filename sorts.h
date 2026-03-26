//
// Created by bopet on 19/03/2026.
//

#ifndef PROGRAMMING_LABS_SORTS_H
#define PROGRAMMING_LABS_SORTS_H
#include <string>
#include <vector>
#include <ostream>
#include <iostream>

// Simple algorithm: Insertion sort
void insertionSort(std::vector<std::string>& arr, int left, int right, bool demo = false, std::ostream& out = std::cout);

// Efficient algorithm 1: Quicksort (Hoare partition, median-of-three)
void quickSort(std::vector<std::string>& arr, int low, int high, bool demo = false, std::ostream& out = std::cout);

// Efficient algorithm 2: Merge sort (top-down)
void mergeSort(std::vector<std::string>& arr, int left, int right, bool demo = false, std::ostream& out = std::cout);

// Combined algorithm (Quicksort + Insertion sort for small subarrays)
void combinedSort(std::vector<std::string>& arr, int low, int high, int threshold);

// Library sort
void librarySort(std::vector<std::string>& arr);

// Reusable Hoare Partition implementation
int hoarePartition(std::vector<std::string>& arr, int low, int high, bool demo = false, std::ostream& out = std::cout);

#endif //PROGRAMMING_LABS_SORTS_H