#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    WSADATA wsData;

    if (WSAStartup(MAKEWORD(2, 2), &wsData) != 0) {
        cout << "WSA Startup Failed\n";
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == INVALID_SOCKET) {
        cout << "Socket creation failed. Error: " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }

    cout << "Socket created successfully!\n";

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(
            serverSocket,
            (sockaddr*)&serverAddr,
            sizeof(serverAddr)
        ) == SOCKET_ERROR)
    {
        cout << "Bind failed. Error: " << WSAGetLastError() << "\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "Bind successful!\n";
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        cout << "Listen failed. Error: " << WSAGetLastError() << "\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "Server listening on port 8080...\n";
    cout << "Run the client with this computer's LAN IP, for example: client.exe 192.168.1.10\n";
    cout << "Waiting for a client...\n";

    sockaddr_in clientAddr{};
    int clientAddrSize = sizeof(clientAddr);
    SOCKET clientSocket = accept(
        serverSocket,
        (sockaddr*)&clientAddr,
        &clientAddrSize
    );

    if (clientSocket == INVALID_SOCKET)
    {
        cout << "Accept failed. Error: " << WSAGetLastError() << "\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "Client connected from " << inet_ntoa(clientAddr.sin_addr) << "!\n";

    char buffer[1024] = {};

    int bytesReceived = recv(
        clientSocket,
        buffer,
        sizeof(buffer) - 1,
        0
    );

    if (bytesReceived == SOCKET_ERROR) {
        cout << "Receive failed. Error: " << WSAGetLastError() << "\n";
    } else if (bytesReceived == 0) {
        cout << "Client closed the connection without sending data.\n";
    } else {
        buffer[bytesReceived] = '\0';
        cout << "Received: " << buffer << endl;
        cout << "Bytes received: " << bytesReceived << endl;
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
