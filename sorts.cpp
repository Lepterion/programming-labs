//
// Created by bopet on 19/03/2026.
//

#include "sorts.h"
#include "utils.h"
#include <iostream>
#include <algorithm>

void insertionSort(std::vector<std::string>& arr, int left, int right, bool demo, std::ostream& out) {
    for (int i = left + 1; i <= right; ++i) {
        std::string key = arr[i];
        int j = i - 1;

        while (j >= left && compareDesc(key, arr[j])) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }

        arr[j + 1] = key;

        if (demo) {
            out << "Insertion Sort step: ";
            printArray(arr, out);
        }
    }
}

int hoarePartition(std::vector<std::string>& arr, int low, int high, bool demo, std::ostream& out) {
    int mid = low + (high - low) / 2;

    // Median-of-three
    if (compareDesc(arr[mid], arr[low])) std::swap(arr[low], arr[mid]);
    if (compareDesc(arr[high], arr[low])) std::swap(arr[low], arr[high]);
    if (compareDesc(arr[high], arr[mid])) std::swap(arr[mid], arr[high]);

    std::string pivot = arr[mid];
    int i = low - 1;
    int j = high + 1;

    while (true) {
        do { i++; } while (compareDesc(arr[i], pivot));
        do { j--; } while (compareDesc(pivot, arr[j]));

        if (i >= j) return j;

        std::swap(arr[i], arr[j]);
        if (demo) {
            out << "Quicksort swap: ";
            printArray(arr, out);
        }
    }
}

void quickSort(std::vector<std::string>& arr, int low, int high, bool demo, std::ostream& out) {
    if (low < high) {
        int pi = hoarePartition(arr, low, high, demo, out);
        quickSort(arr, low, pi, demo, out);
        quickSort(arr, pi + 1, high, demo, out);
    }
}

void merge(std::vector<std::string>& arr, int left, int mid, int right, bool demo, std::ostream& out) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    std::vector<std::string> L(n1), R(n2);

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (compareDesc(L[i], R[j])) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }

        k++;
    }

    while (i < n1) { arr[k] = L[i]; i++; k++; }
    while (j < n2) { arr[k] = R[j]; j++; k++; }

    if (demo) {
        out << "Merge Sort merge: ";
        printArray(arr, out);
    }
}

void mergeSort(std::vector<std::string>& arr, int left, int right, bool demo, std::ostream& out) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid, demo, out);
    mergeSort(arr, mid + 1, right, demo, out);
    merge(arr, left, mid, right, demo, out);
}

void combinedSort(std::vector<std::string>& arr, int low, int high, int threshold) {
    if (high - low + 1 <= threshold) {
        insertionSort(arr, low, high, false, std::cout);
        return;
    }

    if (low < high) {
        int pi = hoarePartition(arr, low, high, false, std::cout);
        combinedSort(arr, low, pi, threshold);
        combinedSort(arr, pi + 1, high, threshold);
    }
}

void librarySort(std::vector<std::string>& arr) {
    std::ranges::sort(arr, compareDesc);
}