#include "Core/Application.h"
#include "Core/Window.h"

#include "pch.h"


Application* Application::s_Instance = nullptr;
Application::Application() {
    m_Window = std::make_unique<Window>();
    LoadDotenv("../.env");
    std::cout << "Loading environment variables" << std::endl;
}
Application* Application::GetInstance() {
    if(s_Instance == nullptr) {
        s_Instance = new Application();
    }
    return s_Instance;
}

void Application::Run(std::function<void()> callback) {
    // Thiết lập backends
    ImGui_ImplGlfw_InitForOpenGL(m_Window->GetWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
    while (!m_Window->WindowShouldClose()) {
        
        m_Window->Update();
        // Khởi tạo frame mới cho ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Tạo cửa sổ ImGui
        ImGui::Begin("Main window", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
        ImGui::SetWindowSize(ImVec2(m_Window->GetWidth(), m_Window->GetHeight()));
        ImGui::SetWindowPos(ImVec2(0, 0));

        callback();
        

        ImGui::End();
        // Kết thúc frame và render
        ImGui::Render();
        glfwGetFramebufferSize(m_Window->GetWindow(), &m_Window->GetProps().Width, &m_Window->GetProps().Height);
        glViewport(0, 0, m_Window->GetProps().Width, m_Window->GetProps().Height);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(m_Window->GetWindow());

    }

    // Dọn dẹp ImGui và GLFW
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
std::vector<std::string> Application::LoadFiles (const std::string& directoryPath) {
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

void Application::LoadDotenv(const std::string& filepath)  {
    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Could not open the file!" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Xóa khoảng trắng ở đầu và cuối dòng
        line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
        line.erase(std::find_if(line.rbegin(), line.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), line.end());

        // Bỏ qua các dòng trống hoặc comment
        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::istringstream iss(line);
        std::string key;
        if (std::getline(iss, key, '=')) {
            std::string value;
            if (!std::getline(iss, value)) {
                value = "";
            }

            // Xóa khoảng trắng ở đầu và cuối key và value
            key.erase(key.begin(), std::find_if(key.begin(), key.end(), [](unsigned char ch) {
                return !std::isspace(ch);
            }));
            key.erase(std::find_if(key.rbegin(), key.rend(), [](unsigned char ch) {
                return !std::isspace(ch);
            }).base(), key.end());

            value.erase(value.begin(), std::find_if(value.begin(), value.end(), [](unsigned char ch) {
                return !std::isspace(ch);
            }));
            value.erase(std::find_if(value.rbegin(), value.rend(), [](unsigned char ch) {
                return !std::isspace(ch);
            }).base(), value.end());

            // Đặt biến môi trường ngay cả khi giá trị rỗng
            #if defined(_WIN32) || defined(_WIN64)
                if (_putenv_s(key.c_str(), value.c_str()) != 0) {
                    std::cerr << "Error setting environment variable: " << key << std::endl;
                }
            #else
                if (setenv(key.c_str(), value.c_str(), 1) != 0) {
                    std::cerr << "Error setting environment variable: " << key << std::endl;
                }
            #endif
        }
    }

    file.close();

}

void Application::ShutDown() {
   
    m_Window->ShutDown();
    Client::GetInstance()->ShutDown();
    delete s_Instance;
    s_Instance = nullptr;
    std::cout << "Close Application" << std::endl;
}


int Application::GetWindowWidth() {
    return m_Window->GetWidth();
}
int Application::GetWindowHeight() {
    return m_Window->GetHeight();
}
