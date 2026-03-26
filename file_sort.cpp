#include "file_sort.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <limits>

using namespace std;

uint32_t calculateElementSize(uint32_t maxSenders, uint32_t maxFrames) {
    uint32_t packetSize = 8 + 8 + 1 + 4 + (maxFrames * 2);
    uint32_t combinedSize = 8 + 4 + (maxSenders * packetSize);
    return combinedSize;
}

string generateAudioPacketsFile() {
    uint32_t count, maxSenders, maxFrames;
    cout << "\n--- Audio Packets File Generation ---\n";
    cout << "Enter the number of packets (e.g., 100): ";
    cin >> count;
    cout << "Enter the maximum number of senders per packet (maxSenders): ";
    cin >> maxSenders;
    cout << "Enter the maximum number of frames per sender (maxFrames): ";
    cin >> maxFrames;

    string filename = "audio_dump_" + to_string(time(nullptr)) + ".bin";
    ofstream file(filename, ios::binary);

    if (!file.is_open()) {
        cerr << "Error creating file!\n";
        return "";
    }

    FileHeader header;
    memcpy(header.magic, "DISC", 4);
    header.elementSize = calculateElementSize(maxSenders, maxFrames);
    header.elementCount = count;
    header.maxSenders = maxSenders;
    header.maxFrames = maxFrames;

    file.write(reinterpret_cast<const char*>(&header), sizeof(FileHeader));

    vector<char> buffer(header.elementSize, 0);

    for (uint32_t i = 0; i < count; ++i) {
        int64_t timestamp = (rand() % 90000) + 10000;
        int32_t senders = rand() % (maxSenders + 1);

        memcpy(buffer.data(), &timestamp, sizeof(int64_t));
        memcpy(buffer.data() + sizeof(int64_t), &senders, sizeof(int32_t));

        file.write(buffer.data(), header.elementSize);
    }

    file.close();
    cout << "File '" << filename << "' has been successfully generated!\n";
    cout << "Size of one record: " << header.elementSize << " bytes.\n";
    return filename;
}

void readBuffer(fstream& file, int index, vector<char>& buffer, uint32_t elementSize) {
    long offset = sizeof(FileHeader) + static_cast<long>(index) * elementSize;
    file.seekg(offset, ios::beg);
    file.read(buffer.data(), elementSize);
}

void writeBuffer(fstream& file, int index, const vector<char>& buffer, uint32_t elementSize) {
    long offset = sizeof(FileHeader) + static_cast<long>(index) * elementSize;
    file.seekp(offset, ios::beg);
    file.write(buffer.data(), elementSize);
}

int64_t getTimestamp(const vector<char>& buffer) {
    int64_t ts;
    memcpy(&ts, buffer.data(), sizeof(int64_t));
    return ts;
}

void sortAudioPacketsFileInPlace(string filename, const string& lastGeneratedFilename) {
    if (filename.empty()) {
        if (lastGeneratedFilename.empty()) {
            cout << "Error: No file generated in this session, and no filename provided!\n";
            return;
        }

        filename = lastGeneratedFilename;
        cout << "Using the last generated file: " << filename << "\n";
    }

    fstream file(filename, ios::in | ios::out | ios::binary);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << "\n";
        return;
    }

    FileHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(FileHeader));

    if (strncmp(header.magic, "DISC", 4) != 0) {
        cerr << "Invalid file format (magic prefix mismatch)!\n";
        return;
    }

    cout << "Starting sort... Total records: " << header.elementCount << "\n";

    vector<char> keyBuffer(header.elementSize);
    vector<char> prevBuffer(header.elementSize);

    for (uint32_t i = 1; i < header.elementCount; ++i) {
        readBuffer(file, i, keyBuffer, header.elementSize);
        int64_t keyTimestamp = getTimestamp(keyBuffer);

        int j = i - 1;
        readBuffer(file, j, prevBuffer, header.elementSize);

        while (j >= 0 && getTimestamp(prevBuffer) > keyTimestamp) {
            writeBuffer(file, j + 1, prevBuffer, header.elementSize);
            j = j - 1;
            if (j >= 0) {
                readBuffer(file, j, prevBuffer, header.elementSize);
            }
        }

        writeBuffer(file, j + 1, keyBuffer, header.elementSize);
    }

    file.close();
    cout << "File sorting completed successfully!\n";
}

void printAudioPacketsFile(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) return;

    FileHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(FileHeader));

    if (strncmp(header.magic, "DISC", 4) != 0) return;

    vector<char> buffer(header.elementSize);
    int displayLimit = min((uint32_t)10, header.elementCount);

    cout << "\nFirst " << displayLimit << " records of file '" << filename << "':\n";
    for (int i = 0; i < displayLimit; ++i) {
        file.read(buffer.data(), header.elementSize);

        int64_t timestamp;
        int32_t senders;
        memcpy(&timestamp, buffer.data(), sizeof(int64_t));
        memcpy(&senders, buffer.data() + sizeof(int64_t), sizeof(int32_t));

        cout << "[" << i << "] Timestamp: " << timestamp << " | Senders count: " << senders << "\n";
    }

    file.close();
}