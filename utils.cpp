//
// Created by bopet on 19/03/2026.
//

#include "utils.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <ctime>

bool compareDesc(const std::string& a, const std::string& b) {
    if (a.length() != b.length()) {
        return a.length() > b.length();
    }

    return a > b;
}

std::string generateRandomString(int length) {
    const std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::string result;
    for (int i = 0; i < length; ++i) {
        result += chars[rand() % chars.length()];
    }

    return result;
}

std::vector<std::string> generateRandomArray(int size) {
    std::vector<std::string> arr(size);
    for (int i = 0; i < size; ++i) {
        arr[i] = generateRandomString((rand() % 5) + 1); // [1; 5] length
    }

    return arr;
}

std::vector<std::string> generateAlmostSortedArray(int size, bool correctOrder) {
    std::vector<std::string> arr = generateRandomArray(size);
    std::ranges::sort(arr, compareDesc);

    if (!correctOrder) {
        std::ranges::reverse(arr); // Make incorrect order
    }

    // Random shuffle
    int swaps = size * 0.05; // 5% chaos
    for (int i = 0; i < swaps; ++i) {
        int idx1 = rand() % size;
        int idx2 = rand() % size;
        std::swap(arr[idx1], arr[idx2]);
    }

    return arr;
}

void printArray(const std::vector<std::string>& arr, std::ostream& out) {
    for (const auto& s : arr) {
        out << s << " ";
    }

    out << "\n";
}

std::vector<std::string> readFromFile(const std::string& filename) {
    std::vector<std::string> arr;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Failed to open file '" << filename << "'.\n";
        return arr;
    }

    std::string word;
    while (file >> word)
        arr.push_back(word);

    return arr;
}

void writeToFile(const std::vector<std::string>& arr, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& s : arr)
            file << s << "\n";
    }
}

std::string generateLogFilename(const std::string& prefix) {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << prefix << "_" << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S") << ".txt";
    return oss.str();
}