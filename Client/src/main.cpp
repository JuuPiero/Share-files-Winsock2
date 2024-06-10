#include "pch.h"

int main(int argc, char* argv[]) {
    
    auto app = Application::GetInstance();
    
    GLuint basicFileTexture = BasicFileTexture();
    app->Run([&basicFileTexture]() {
        FileViewModel::GetInstance()->Update();

        if(ImGui::Button("Connect")) {
            Client::GetInstance()->Connect();
        }

        if(ImGui::InputTextMultiline(" ", FileViewModel::GetInstance()->searchKeywordsBuffer , 100, ImVec2(0, 35))) {}
        ImGui::SameLine();
        if(ImGui::Button("Search")) {
            FileViewModel::GetInstance()->searchResults.clear();
            FileViewModel::GetInstance()->OnSearch(std::string(FileViewModel::GetInstance()->searchKeywordsBuffer));
        }

        ImGui::Text("Search Result(s) :");
        if (ImGui::BeginTable("Search result", 10)) {
            auto searchResults = FileViewModel::GetInstance()->searchResults;
            for (int i = 0; i < searchResults.size(); i++) {
                // Tạo hàng mới khi cần thiết
                if (i % 10 == 0) ImGui::TableNextRow();
                // Thiết lập chỉ số cột
                ImGui::TableSetColumnIndex(i % 10);
                // Hiển thị hình ảnh của file
                ImGui::Image((void*)(intptr_t)basicFileTexture, ImVec2(128, 128));
                // Tính toán kích thước văn bản và căn giữa
                ImVec2 textSize = ImGui::CalcTextSize(searchResults[i].second.c_str());
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetColumnWidth() - textSize.x) * 0.5f);
                // Hiển thị tên file
                ImGui::Text("%s", searchResults[i].second.c_str());
                // chủ file
                textSize = ImGui::CalcTextSize("client_id: 1");
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetColumnWidth() - textSize.x) * 0.5f);
                ImGui::Text("client_id: %d", searchResults[i].first);
                //down btn
                textSize = ImGui::CalcTextSize(searchResults[i].second.c_str());
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetColumnWidth() - textSize.x) * 0.5f);
                std::string buttonLabel = "Download##" + std::to_string(i);
                if(ImGui::Button(buttonLabel.c_str())) {
                    FileViewModel::GetInstance()->OnDownload(searchResults[i].first, searchResults[i].second);
                }
            }
        }
        ImGui::EndTable();

        ImGui::Text("Your file(s) :");
        if (ImGui::BeginTable("My Share Files", 10)) {
            for (int i = 0; i < FileViewModel::GetInstance()->files.size(); i++) {
                if (i % FileViewModel::GetInstance()->columns == 0) {
                    ImGui::TableNextRow();
                }
                ImGui::TableSetColumnIndex(i % FileViewModel::GetInstance()->columns);
                ImGui::Image((void*)(intptr_t)basicFileTexture, ImVec2(128, 128));
                ImVec2 textSize = ImGui::CalcTextSize(FileViewModel::GetInstance()->files[i].c_str());
                float cellWidth = ImGui::GetColumnWidth();
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (cellWidth - textSize.x) * 0.5f);
                ImGui::Text("%s", FileViewModel::GetInstance()->files[i].c_str());
            }
        }
        ImGui::EndTable();


        ImGui::Text("File(s) Downloaded :");
        if (ImGui::BeginTable("Table files Downloaded", 10)) {
            for (int i = 0; i < FileViewModel::GetInstance()->filesDownloaded.size(); i++) {
                if (i % FileViewModel::GetInstance()->columns == 0) {
                    ImGui::TableNextRow();
                }
                ImGui::TableSetColumnIndex(i % FileViewModel::GetInstance()->columns);
                ImGui::Image((void*)(intptr_t)basicFileTexture, ImVec2(128, 128));
                ImVec2 textSize = ImGui::CalcTextSize(FileViewModel::GetInstance()->filesDownloaded[i].c_str());
                float cellWidth = ImGui::GetColumnWidth();
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (cellWidth - textSize.x) * 0.5f);
                ImGui::Text("%s", FileViewModel::GetInstance()->filesDownloaded[i].c_str());
            }
        }
        ImGui::EndTable();
    });

    app->ShutDown();
    return 0;
}