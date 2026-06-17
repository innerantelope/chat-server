#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main(int argc, char* argv[]) {
    string serverIp = "127.0.0.1";
    if (argc > 1) {
        serverIp = argv[1];
    }

    WSADATA wsData;

    if (WSAStartup(MAKEWORD(2, 2), &wsData) != 0) {
        cout << "WSA Startup Failed\n";
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        cout << "Socket creation failed. Error: " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);

    serverAddr.sin_addr.s_addr = inet_addr(serverIp.c_str());
    if (serverAddr.sin_addr.s_addr == INADDR_NONE) {
        cout << "Invalid server IP address: " << serverIp << "\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    if (connect(
            clientSocket,
            (sockaddr*)&serverAddr,
            sizeof(serverAddr)
        ) == SOCKET_ERROR)
    {
        cout << "Connection failed. Error: " << WSAGetLastError() << "\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    cout << "Connected to server at " << serverIp << ":8080!\n";
    string message = "Hello Server";

    int bytesSent = send(
        clientSocket,
        message.c_str(),
        static_cast<int>(message.size()),
        0
    );

    if (bytesSent == SOCKET_ERROR) {
        cout << "Message send failed. Error: " << WSAGetLastError() << "\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    cout << "Message sent!\n";

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
