#include <unordered_map>
#include <iostream>
#include <string>
#include <WinSock2.h>

int main() {
    std::unordered_map<std::string, SOCKET> map;

    for (size_t i = 0; i < 100; i++) {
        std::cout << (map[std::to_string(i)] == 0) << std::endl;
    }
    
    

    return 0;
}