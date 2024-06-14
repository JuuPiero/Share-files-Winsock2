#pragma once
#include <vector>
#include <iostream>
class FileViewModel {
public:
    ~FileViewModel() = default;
    static FileViewModel* GetInstance();

    void OnSearch(std::string keywords);
    void OnDownload(int ownerId, std::string filename);
    void Update();
private:
    FileViewModel();
public:
    static FileViewModel* s_Instance;
    char searchKeywordsBuffer[100] = "";
    std::vector<std::string> files;
    std::vector<std::string> filesDownloaded;
    std::vector<std::pair<int, std::string>> searchResults;
private:

};
