#pragma once

#include "pch.h"
class Window;
class Application {
public:
    static Application* GetInstance();

    void Run(std::function<void()> callback = []() {});
    void ShutDown();

    int GetWindowWidth();
    int GetWindowHeight();
    std::vector<std::string> LoadFiles(const std::string& directoryPath);

private:
    void LoadDotenv(const std::string& filepath);
    Application();
    ~Application() = default;
private:
    std::shared_ptr<Window> m_Window;
    static Application* s_Instance;
};