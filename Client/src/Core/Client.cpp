#include "Core/Client.h"
#include "pch.h"
#include <sstream>
Client* Client::s_Instance = nullptr;
Client::Client(): m_ServerPort(atoi(getenv("SERVER_PORT"))), m_ServerAddress(getenv("SERVER_ADDRESS")) {
    std::cout << m_ServerPort << std::endl;
    std::cout << m_ServerAddress << std::endl;
    int iResult = WSAStartup(MAKEWORD(2, 2), &m_WsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed: " << iResult << std::endl;
        ShutDown();
        return;
    }
    m_ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_ClientSocket == INVALID_SOCKET) {
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
        ShutDown();
        return;
    }

    m_ServerAddr.sin_family = AF_INET;
    m_ServerAddr.sin_addr.s_addr = inet_addr(m_ServerAddress); // Địa chỉ IP của máy chủ trong mạng cục bộ
    m_ServerAddr.sin_port = htons(m_ServerPort);

    int connectResult = connect(m_ClientSocket, (struct sockaddr*)&m_ServerAddr, sizeof(m_ServerAddr));
    if (connectResult == SOCKET_ERROR) {
        std::cout << "connect failed: " << WSAGetLastError() << std::endl;
        closesocket(m_ClientSocket);
        ShutDown();
        return;
    }

    m_ReceiveThread = std::thread(&Client::ReceiveMessages, this);
    m_ReceiveThread.detach();
}


Client* Client::GetInstance() {
    if (s_Instance == nullptr) {
        s_Instance = new Client();
    }
    return s_Instance;
}

void Client::ReceiveMessages() {
    char recvbuf[DEFAULT_BUFLEN];
    do {
        iResult = recv(m_ClientSocket, recvbuf, DEFAULT_BUFLEN, 0);
        if (iResult > 0) {
            recvbuf[iResult] = '\0';
            std::cout << "Message from server: " << recvbuf << std::endl;
            json responseJson = json::parse(std::string(recvbuf));
            std::cout << responseJson.dump(4) << std::endl;

            int statusCode = responseJson["status"];
            std::cout << "StatusCode: " << statusCode << std::endl;

            switch (statusCode) {
                case StatusCode::CONNECT_SUCCESS: {
                    Client::GetInstance()->SetId(responseJson["client_id"]);
                    break;
                }

                case StatusCode::REQUEST_SEARCH: {
                    std::string keywords = responseJson["keywords"];
                    std::cout << "A client with id : " <<  responseJson["sender_id"]  << " request search file with keywords: " << keywords << std::endl;
                    //TODO:
                    json newjsonResponse;
                    newjsonResponse["command"] = Command::RESPONSE_SEARCH;
                    newjsonResponse["client_id"] = GetId();
                    newjsonResponse["sender_id"] = responseJson["sender_id"];

                    auto files = FileViewModel::GetInstance()->files;
                    int count = 0;
                    for (size_t i = 0; i < files.size(); i++) {
                        if(files[i].find(keywords) != std::string::npos) {
                            newjsonResponse["files"][count] = files[i];
                            count++;
                        }
                    }
                    newjsonResponse["count"] = count;
                    SendCommand(newjsonResponse.dump());
                    break;
                }

                case StatusCode::SEARCH_SUCCESS: {
                    int count = responseJson["count"];
                    int ownerId = responseJson["client_id"];
                    if(count) {
                        for (size_t i = 0; i < count; i++) {
                            FileViewModel::GetInstance()->searchResults.emplace_back(ownerId, responseJson["files"][i]);
                        }
                    }
                    break;
                }

               
               
                default:
                    break;
            }
        } 
        else if (iResult == 0) {
            std::cout << "Connection closed by server" << std::endl;
            break;
        } 
        else {
            std::cout << "recv failed: " << WSAGetLastError() << std::endl;
            break;
        }
    } while (iResult > 0);
}
void Client::Connect() {
    json requestData;
    requestData["command"] = Client::Command::CONNECT;
    Client::GetInstance()->SendCommand(requestData.dump());
}

void Client::SendCommand(const std::string& command) {
    send(m_ClientSocket, command.c_str(), command.size(), 0);
}
void Client::ShutDown() {
    std::cout << "Free Client Instance" << std::endl;
    closesocket(m_ClientSocket);
    WSACleanup();
    delete s_Instance;
    s_Instance = nullptr;
}
    