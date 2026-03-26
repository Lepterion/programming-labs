//
// Created by bopet on 19/03/2026.
//

#ifndef FILE_SORT_H
#define FILE_SORT_H

#include <string>
#include <vector>
#include <cstdint>

// Спецпрефікс файлу
#pragma pack(push, 1) // Перший раз у житті бачу таке, але без цього траплялися аномалії і не працювало як треба, мені таке Gemini 3.1 Pro показав
struct FileHeader {
    char magic[4];
    uint32_t elementSize;
    uint32_t elementCount;
    uint32_t maxSenders;
    uint32_t maxFrames;
};
#pragma pack(pop)

std::string generateAudioPacketsFile();

// Сортування за timestamp
void sortAudioPacketsFileInPlace(std::string filename, const std::string& lastGeneratedFilename);

void printAudioPacketsFile(const std::string& filename);

#endif // FILE_SORT_H