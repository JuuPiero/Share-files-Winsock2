#include "pch.h"

int main(int argc, char* argv[]) {
    


    
    auto app = Application::GetInstance();
    std::cout << "Files in directory '" << getenv("STORAGE_DIR") << "':" << std::endl;
    for (const auto& file : FileViewModel::GetInstance()->files) {
        std::cout << file << std::endl;
    }


    GLuint myTexture = LoadTextureFromFile("../assets/file.png");
    app->Run([&myTexture]() {
        // int tableRowCount = FileViewModel::GetInstance()->files.size() / tableColumnCount;
        FileViewModel::GetInstance()->Update();

        if(ImGui::Button("Connect")) {
            Client::GetInstance()->Connect();
        }

        if(ImGui::InputTextMultiline(" ", FileViewModel::GetInstance()->searchKeywordsBuffer , 100, ImVec2(0, 35))) {

        }
        ImGui::SameLine();
        if(ImGui::Button("Search")) {
            FileViewModel::GetInstance()->OnSearch(std::string(FileViewModel::GetInstance()->searchKeywordsBuffer));
        }
        // int columns = FileViewModel::GetInstance()->files.size() > 10 ? 10 : FileViewModel::GetInstance()->files.size();
        // int rows = std::ceil(double(FileViewModel::GetInstance()->files.size() * 1.0f) / 10);
        // std::cout << rows << std::endl;
        // std::cout << columns << std::endl;

        ImGui::Text("Search Result(s) :");
        if (ImGui::BeginTable("TableExample", 10)) {
            auto searchResults = FileViewModel::GetInstance()->searchResults;
            for (int i = 0; i < searchResults.size(); i++) {
                // Tạo hàng mới khi cần thiết
                if (i % 10 == 0) {
                    ImGui::TableNextRow();
                }
                // Thiết lập chỉ số cột
                ImGui::TableSetColumnIndex(i % 10);
        
                // Hiển thị hình ảnh của file
                ImGui::Image((void*)(intptr_t)myTexture, ImVec2(128, 128));
        
                // Tính toán kích thước văn bản và căn giữa nó
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
                ImGui::Button("Download");
            }
        }
        ImGui::EndTable();

        ImGui::Text("Your file(s) :");
        if (ImGui::BeginTable("TableExample", 10)) {
            for (int i = 0; i < FileViewModel::GetInstance()->files.size(); i++) {
                // Tạo hàng mới khi cần thiết
                if (i % FileViewModel::GetInstance()->columns == 0) {
                    ImGui::TableNextRow();
                }
                // Thiết lập chỉ số cột
                ImGui::TableSetColumnIndex(i % FileViewModel::GetInstance()->columns);
        
                // Hiển thị hình ảnh của file
                ImGui::Image((void*)(intptr_t)myTexture, ImVec2(128, 128));
        
                // Tính toán kích thước văn bản và căn giữa nó
                ImVec2 textSize = ImGui::CalcTextSize(FileViewModel::GetInstance()->files[i].c_str());
                float cellWidth = ImGui::GetColumnWidth();
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (cellWidth - textSize.x) * 0.5f);
            
                // Hiển thị tên file
                ImGui::Text("%s", FileViewModel::GetInstance()->files[i].c_str());
            }
        }
        ImGui::EndTable();

        
    });

    app->ShutDown();
    return 0;
}