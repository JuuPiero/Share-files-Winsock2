#include "FileViewModel.h"
#include "pch.h"
FileViewModel* FileViewModel::s_Instance = nullptr;
FileViewModel::FileViewModel() {
    files = Application::GetInstance()->LoadFiles(getenv("STORAGE_DIR"));
}

FileViewModel* FileViewModel::GetInstance() {
    if(s_Instance == nullptr) {
        s_Instance = new FileViewModel();
    }
    return s_Instance;
}

void FileViewModel::Update() {
    files = Application::GetInstance()->LoadFiles(getenv("STORAGE_DIR"));
    filesDownloaded = Application::GetInstance()->LoadFiles(getenv("DOWNLOAD_DIR"));
    if(files.size() <= 10) {
        columns = files.size();
        rows = 1;
    }
    else {
        columns = 10;
        rows = files.size() / 10 + 1;
    }
}

void FileViewModel::OnSearch(std::string keywords) {
    json requestData;
    requestData["command"] = Client::Command::SEARCH;
    requestData["sender_id"] = Client::GetInstance()->GetId();
    requestData["keywords"] = keywords;
    Client::GetInstance()->SendCommand(requestData.dump());
}
void FileViewModel::OnDownload(int ownerId, std::string filename) {
    json requestData;
    requestData["command"] = Client::Command::DOWNLOAD;
    requestData["client_id"] = Client::GetInstance()->GetId();
    requestData["owner_id"] = ownerId;
    requestData["filename"] = filename;
    Client::GetInstance()->SendCommand(requestData.dump());


}
