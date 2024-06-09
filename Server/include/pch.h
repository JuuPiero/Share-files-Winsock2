#pragma once
// #include "imgui.h"
// #include "imgui_impl_glfw.h"
// #include "imgui_impl_opengl3.h"
// #include "glad/GL.h"
// #include <glfw3.h>
#define MAX_CLIENTS 1024
#include <cstdlib>
#include <stdlib.h>

#include <vector>
#include <unordered_map>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <time.h>
#include <map>
#include <thread>
#include <algorithm>
#include <memory>
#include <functional>
#include <ctime>
#include <iomanip>
#include <random>
#include <WinSock2.h>
#include <Windows.h>

#include "spdlog/spdlog.h"

#include "Core/Server.h"
#include "Core/Application.h"
#include "StatusCode.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;


inline std::string GenerateUUID() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    std::uniform_int_distribution<> dis2(8, 11);
    std::stringstream ss;
    ss << std::hex;

    for (int i = 0; i < 8; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (int i = 0; i < 4; i++) {
        ss << dis(gen);
    }
    ss << "-4"; // 4 cố định cho UUID phiên bản 4
    for (int i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    ss << dis2(gen); // y là một trong các giá trị 8, 9, a, b
    for (int i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (int i = 0; i < 12; i++) {
        ss << dis(gen);
    }
    return ss.str();
}

inline uint32_t GetId() {
    static uint32_t id = 0;
    return ++id;
}