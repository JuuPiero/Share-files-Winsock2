#pragma once


#include "pch.h"

class Server {
public:
    enum Command {
        LOGIN = 0,
        LOGOUT = 1,
        SEARCH = 20,
        CONNECT = 10,
        RESPONSE_SEARCH = 21,
        DOWNLOAD = 30,
        SEND_FILE = 31
    };
public:
    static Server* GetInstance();
    void ShutDown();
    void Run();
    ~Server() = default;


    void OnClientConnect(uint32_t clientId);
    void OnClientSearch(uint32_t clientId, std::string keywords);

private:
    void ClientHandler(uint32_t clientSocketId);

private:
    Server();
    static Server* s_Insntance;
    WSADATA m_WsaData;
    SOCKET m_ListenSocket;
    // std::vector<SOCKET> m_ClientSockets;
    std::unordered_map<uint32_t, SOCKET> m_ClientSockets;
    sockaddr_in m_ServerAddr;
    uint32_t m_Port;

    int iResult;
};
