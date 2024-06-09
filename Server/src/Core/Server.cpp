#include "Core/Server.h"
#include "spdlog/spdlog.h"
#include "pch.h"
Server* Server::s_Insntance = nullptr;

Server::Server(): m_Port(atoi(getenv("PORT"))) {
    // Initialize Winsock
    int iResult = WSAStartup(MAKEWORD(2, 2), &m_WsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed: " << iResult << std::endl;
        ShutDown();
        return;
    }
    // Create a socket for listening
    m_ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_ListenSocket == INVALID_SOCKET) {
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
        ShutDown();
        return;
    }

    // Setup server address
    m_ServerAddr.sin_family = AF_INET;
    m_ServerAddr.sin_addr.s_addr = INADDR_ANY; // cái này lắng nghe mọi máy từ mạng cục bộ
    m_ServerAddr.sin_port = htons(m_Port);

    // Bind socket
    iResult = bind(m_ListenSocket, (struct sockaddr*)&m_ServerAddr, sizeof(m_ServerAddr));
    if (iResult == SOCKET_ERROR) {
        std::cout << "bind failed: " << WSAGetLastError() << std::endl;
        ShutDown();
        return;
    }
    // Listen on socket
    iResult = listen(m_ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        std::cout << "listen failed: " << WSAGetLastError() << std::endl;
        closesocket(m_ListenSocket);
        WSACleanup();
        return;
    }
    spdlog::info("Server listening on port: {}", m_Port);
}

Server* Server::GetInstance() {
    if (s_Insntance == nullptr) {
        s_Insntance = new Server();
    }
    return s_Insntance;
}
void Server::ShutDown() {
    closesocket(m_ListenSocket);
    WSACleanup();
    delete s_Insntance;
    s_Insntance = nullptr;
}

void Server::Run() {
    // Accept a client socket
    SOCKET clientSocket;
    clientSocket = accept(m_ListenSocket, nullptr, nullptr);
    if (clientSocket == INVALID_SOCKET) {
        std::cout << "accept failed: " << WSAGetLastError() << std::endl;
        closesocket(m_ListenSocket);
        WSACleanup();
        return;
    }
    
    // std::string clientSocketId = GenerateUUID();
    uint32_t clientSocketId = GetId();
    // m_ClientSockets.push_back(clientSocket);
    // m_ClientSockets.emplace(clientSocketId, clientSocket);
    m_ClientSockets[clientSocketId] = clientSocket;
   
    
    // Create a thread to handle client
    std::thread handlerThread(&Server::ClientHandler, this, clientSocketId);
    handlerThread.detach(); // Detach thread to run independently
}

void Server::ClientHandler(uint32_t clientSocketId) {
    char recvbuf[DEFAULT_BUFLEN];
    SOCKET clientSocket = m_ClientSockets[clientSocketId];
    
    spdlog::info("A client connected to server: {}",  clientSocketId);
   

    do {
        iResult = recv(clientSocket, recvbuf, DEFAULT_BUFLEN, 0);
        if (iResult > 0) {
            
            recvbuf[iResult] = '\0';
            //parse data from client
            json requestData = json::parse(std::string(recvbuf));
            std::cout << "message from client: " << std::endl;
            std::cout << requestData.dump(4) << std::endl;
            int command = requestData["command"];

            if(command == Command::SEARCH) {
                std::cout << "A client with id : " << requestData["sender_id"] << " request search file with keywords: " << requestData["keywords"] << std::endl;

                json responseJson;
                responseJson["status"] = StatusCode::REQUEST_SEARCH;
                responseJson["keywords"] = requestData["keywords"];
                responseJson["sender_id"] = requestData["sender_id"];

                for (auto& client : m_ClientSockets) {
                    if(client.second != clientSocket) {
                        iResult = send(client.second, responseJson.dump().c_str(), responseJson.dump().size(), 0);
                        if (iResult == SOCKET_ERROR) {
                            std::cout << "send failed: " << WSAGetLastError() << std::endl;
                        }
                    }
                }
            }
            else if(command == Command::RESPONSE_SEARCH) {
                json responseJson;
                int count = requestData["count"];
                std::cout << "Client with id: " << requestData["client_id"] << " has " << count << " result file(s)" << std::endl;
                if(count) {
                    responseJson["status"] = StatusCode::SEARCH_SUCCESS;
                    responseJson["count"] = count;
                    responseJson["client_id"] = requestData["client_id"];
                    responseJson["sender_id"] = requestData["sender_id"];
                    
                    for (size_t i = 0; i < count; i++) {
                        std::cout << requestData["files"][i] << std::endl;
                        responseJson["files"][i] = requestData["files"][i];
                    }
                    if(m_ClientSockets[responseJson["sender_id"]]) {
                        iResult = send(m_ClientSockets[responseJson["sender_id"]], responseJson.dump().c_str(), responseJson.dump().size(), 0);
                        if (iResult == SOCKET_ERROR) {
                            std::cout << "send failed: " << WSAGetLastError() << std::endl;
                        }
                    }
                }
            }
            else if(command = Command::CONNECT) {
                std::cout << "A client connected to server: " << clientSocket << std::endl;
                json responseJson;
                responseJson["status"] = StatusCode::CONNECT_SUCCESS;
                responseJson["client_id"] = clientSocketId;
                iResult = send(clientSocket, responseJson.dump().c_str(), responseJson.dump().size(), 0);
                if (iResult == SOCKET_ERROR) {
                    std::cout << "send failed: " << WSAGetLastError() << std::endl;
                }
            }
            
        } else if (iResult == 0) {
            std::cout << "Connection closed by client" << std::endl;
        } else {
            spdlog::error("A client disconnected : {}", WSAGetLastError());
        }
    } while (iResult > 0);

    closesocket(clientSocket);

    m_ClientSockets.erase(clientSocketId);
    
}

Server::~Server() {}