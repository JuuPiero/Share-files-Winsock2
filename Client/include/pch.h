#pragma once
#define DEFAULT_BUFLEN 1000000
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "glad/GL.h"
#include <glfw3.h>
#include <cstdlib>
#include <stdlib.h>
#include <WinSock2.h>
#include <Windows.h>
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
#include <functional>
#include <cmath>
#include <stb_image.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

// #include <filesystem>
// using fs = std::filesystem;

#include "Core/Window.h"
#include "Core/Client.h"
#include "Core/Application.h"
#include "Core/StatusCode.h"
#include "Core/UI.h"
#include "base64.h"
#include "FileViewModel.h"