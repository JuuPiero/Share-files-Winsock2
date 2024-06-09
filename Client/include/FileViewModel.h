#pragma once
#include <vector>
#include <iostream>
class FileViewModel {
public:
    ~FileViewModel() = default;
    static FileViewModel* GetInstance();

    void OnSearch(std::string keywords);

    void Update();
private:
    FileViewModel();
public:
    static FileViewModel* s_Instance;
    std::vector<std::string> files;
    int rows, columns;
    char searchKeywordsBuffer[100] = "";
    std::vector<std::pair<int, std::string>> searchResults;
private:

};
