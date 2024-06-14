#include "Core/Application.h"
#include "pch.h"

Application* Application::s_Instance = nullptr;

Application* Application::GetInstance() {
    if(s_Instance == nullptr) s_Instance = new Application();
    return s_Instance;
}

void Application::Run(std::function<void()> callback) {
    m_IsRunning = true;

    while(m_IsRunning) {

        Server::GetInstance()->Run();
        
        callback();
    }
}

void Application::ShutDown() {
    Server::GetInstance()->ShutDown();
    m_IsRunning = false;
    delete s_Instance;
    s_Instance = nullptr;
}


Application::Application() {
    spdlog::set_pattern("%^[%H:%M:%S] [thread %t] %v%$");
    SPDLOG_INFO("Created Application");
}

