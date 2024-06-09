#pragma once
#define DEFAULT_BUFLEN 1000000
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


inline uint32_t GetId() {
    static uint32_t id = 0;
    return ++id;
}