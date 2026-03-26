#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <limits>
#include "utils.h"
#include "sorts.h"
#include "explicit_stack_sorting.h"
#include "file_sort.h"

using namespace std;
using namespace std::chrono;

// На жаль, довелося відмовитися від с-style output'у заради здорового глузду та зручності.

#define MEASURE_SORT(SORT_NAME, FUNCTION_CALL) \
    do { \
        arr = testCases[t]; \
        auto start_time = high_resolution_clock::now(); \
        FUNCTION_CALL; \
        auto stop_time = high_resolution_clock::now(); \
        out << left << setw(38) << SORT_NAME << ": " \
            << duration_cast<milliseconds>(stop_time - start_time).count() << " ms\n"; \
    } while(0)

#define DEMO_SORT(SORT_NAME, ...) \
    do { \
        out << "\n--- " << SORT_NAME << " ---\n"; \
        vector<string> arr = original; \
        __VA_ARGS__; \
    } while(0)

void runDemo(ostream& out) {
    int size;
    cout << "Enter array size for demo (5-10 recommended): ";
    cin >> size;

    vector<string> original = generateRandomArray(size);
    out << "\nInitial array:\n";
    printArray(original, out);

    DEMO_SORT("Insertion Sort", insertionSort(arr, 0, arr.size() - 1, true, out));
    DEMO_SORT("Quicksort (Recursive)", quickSort(arr, 0, arr.size() - 1, true, out));
    DEMO_SORT("Quicksort (Iterative Stack)", quickSortIterative(arr, 0, arr.size() - 1, true, out));
    DEMO_SORT("Merge Sort", mergeSort(arr, 0, arr.size() - 1, true, out));
    DEMO_SORT("Combined Sort (Iterative, threshold 3)", 
              combinedSortIterative(arr, 0, arr.size() - 1, 3); 
              printArray(arr, out));
}

void runBenchmark(ostream& out) {
    const int SIZE = 10000;
    out << "\n=== BENCHMARK (Array size: " << SIZE << ") ===\n";

    vector<string> randArr = generateRandomArray(SIZE);
    vector<string> ascArr = generateAlmostSortedArray(SIZE, true);
    vector<string> descArr = generateAlmostSortedArray(SIZE, false);

    vector<vector<string>> testCases = {randArr, ascArr, descArr};
    string caseNames[] = {"Random", "Almost sorted (Correct order)", "Almost sorted (Incorrect order)"};

    for (int t = 0; t < 3; ++t) {
        out << "\nTest: " << caseNames[t] << "\n";
        vector<string> arr;

        MEASURE_SORT("Insertion Sort", insertionSort(arr, 0, arr.size() - 1, false, out));
        MEASURE_SORT("Quicksort (Recursive)", quickSort(arr, 0, arr.size() - 1, false, out));
        MEASURE_SORT("Quicksort (Iterative)", quickSortIterative(arr, 0, arr.size() - 1, false, out));
        MEASURE_SORT("Merge Sort", mergeSort(arr, 0, arr.size() - 1, false, out));
        MEASURE_SORT("Combined Sort (Recursive)", combinedSort(arr, 0, arr.size() - 1, 16));
        MEASURE_SORT("Combined Sort (Iterative)", combinedSortIterative(arr, 0, arr.size() - 1, 16));
        MEASURE_SORT("Library Sort", librarySort(arr));
    }
}

int main() {
    srand(time(nullptr)); 
    
    int choice;
    bool fileOutputMode = false;
    string lastGeneratedFile = ""; 

    do {
        cout << "\n=== Main Menu ===\n"
             << "1. Demonstration mode\n"
             << "2. Benchmark mode\n"
             << "3. Toggle file output logging [" << (fileOutputMode ? "ON" : "OFF") << "]\n"
             << "--- File Sorting (Lab 3b) ---\n"
             << "4. Generate binary file with audio packets\n"
             << "5. Sort binary file (In-Place)\n"
             << "0. Exit\n"
             << "Your choice: ";
        cin >> choice;

        if (choice == 3) {
            fileOutputMode = !fileOutputMode;
        } 
        else if (choice == 1 || choice == 2) {
            if (fileOutputMode) {
                string prefix = (choice == 1) ? "demo" : "benchmark";
                string filename = generateLogFilename(prefix);
                ofstream fout(filename);

                if (fout.is_open()) {
                    cout << "Processing... Results will be written to '" << filename << "'\n";
                    if (choice == 1) runDemo(fout);
                    else runBenchmark(fout);
                    cout << "Successfully written to file!\n";
                } else {
                    cout << "Error: Could not open file for writing!\n";
                }
            } else {
                if (choice == 1) runDemo(cout);
                else runBenchmark(cout);
            }
        }
        else if (choice == 4) {
            lastGeneratedFile = generateAudioPacketsFile();
            if (!lastGeneratedFile.empty()) {
                printAudioPacketsFile(lastGeneratedFile);
            }
        }
        else if (choice == 5) {
            cout << "Enter filename to sort (leave empty to use '"
                 << (lastGeneratedFile.empty() ? "none" : lastGeneratedFile) << "'): ";

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            string inputFilename;
            getline(cin, inputFilename);

            sortAudioPacketsFileInPlace(inputFilename, lastGeneratedFile);

            string fileToPrint = inputFilename.empty() ? lastGeneratedFile : inputFilename;
            printAudioPacketsFile(fileToPrint);
        }

    } while (choice != 0);

    return 0;
}