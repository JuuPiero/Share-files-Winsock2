#include "Core/Server.h"

void Server::OnClientSearch(uint32_t clientId, std::string keywords) {
    std::cout << "A client with id : " << clientId << " request search file with keywords: " << keywords << std::endl;

    json responseJson;
    responseJson["status"] = StatusCode::REQUEST_SEARCH;
    responseJson["keywords"] = keywords;
    responseJson["sender_id"] = clientId;

    for (auto& client : m_ClientSockets) {
        if(client.second != m_ClientSockets[clientId]) {
            iResult = send(client.second, responseJson.dump().c_str(), responseJson.dump().size(), 0);
            if (iResult == SOCKET_ERROR) {
                std::cout << "send failed: " << WSAGetLastError() << std::endl;
            }
        }
    }

}

void Server::OnClientConnect(uint32_t clientId) {
    std::cout << "A client connected to server: " << m_ClientSockets[clientId] << std::endl;
    json responseJson;
    responseJson["status"] = StatusCode::CONNECT_SUCCESS;
    responseJson["client_id"] = clientId;
    iResult = send(m_ClientSockets[clientId], responseJson.dump().c_str(), responseJson.dump().size(), 0);
    if (iResult == SOCKET_ERROR) {
        std::cout << "send failed: " << WSAGetLastError() << std::endl;
    }
}