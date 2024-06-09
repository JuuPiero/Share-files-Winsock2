#include <windows.h>
#include <iostream>
#include <vector>
#include <string>

std::vector<std::string> listFilesInDirectory(const std::string& directoryPath) {
    std::vector<std::string> fileNames;
    std::string searchPath = directoryPath + "\\*";
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(searchPath.c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to find the first file in the directory." << std::endl;
        return fileNames;
    }

    do {
        const std::string fileName = findFileData.cFileName;
        const bool isDirectory = (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

        if (!isDirectory) {
            fileNames.push_back(fileName);
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
    return fileNames;
}

int main() {
    std::string path = "./storages"; // Thay thế bằng đường dẫn thư mục của bạn
    std::vector<std::string> files = listFilesInDirectory(path);

    std::cout << "Files in directory '" << path << "':" << std::endl;
    for (const auto& file : files) {
        std::cout << file << std::endl;
    }

    return 0;
}