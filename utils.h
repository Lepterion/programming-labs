//
// Created by bopet on 19/03/2026.
//

#ifndef PROGRAMMING_LABS_UTILS_H
#define PROGRAMMING_LABS_UTILS_H

#include <vector>
#include <string>
#include <ostream>
#include <iostream>

// Comparator: first by length (longer first), then lexicographically descending
bool compareDesc(const std::string& a, const std::string& b);

// Generate random string of given length
std::string generateRandomString(int length);

// Generate array of random strings
std::vector<std::string> generateRandomArray(int size);

// Generate almost sorted array (in correct or incorrect order)
std::vector<std::string> generateAlmostSortedArray(int size, bool correctOrder);

// Print array to output stream (console or file)
void printArray(const std::vector<std::string>& arr, std::ostream& out = std::cout);

// Read array from file
std::vector<std::string> readFromFile(const std::string& filename);

// Write array to file
void writeToFile(const std::vector<std::string>& arr, const std::string& filename);

// Generate file name with optional prefix
std::string generateLogFilename(const std::string& prefix = "output_log");

#endif //PROGRAMMING_LABS_UTILS_H