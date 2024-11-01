#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int main() {
    
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);

    if (wsaerr != 0) {
        cout << "The Winsock dll not found!" << endl;
        return 0;
    }
    else {
        cout << "The Winsock dll found" << endl;
        cout << "The status: " << wsaData.szSystemStatus << endl;
    }

    
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        cout << "Error at socket():" << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }
    else {
        cout << "Socket is OK!" << endl;
    }

    
    sockaddr_in service;
    service.sin_family = AF_INET;
    InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr.s_addr);
    service.sin_port = htons(55555);

    if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        cout << "bind() failed: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    }
    else {
        cout << "bind() is OK!" << endl;
    }

    if (listen(serverSocket, 1) == SOCKET_ERROR) {
        cout << "listen(): Error listening on socket: " << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }
    else {
        cout << "listen() is OK!, I'm waiting for new connections..." << endl;
    }

    SOCKET acceptSocket = accept(serverSocket, NULL, NULL);
    if (acceptSocket == INVALID_SOCKET) {
        cout << "accept failed: " << WSAGetLastError() << endl;
        WSACleanup();
        return -1;
    }
    else {
        cout << "accept() is OK!" << endl;
    }

   
    while (true) {
        char receiveBuffer[200];
        int rbyteCount = recv(acceptSocket, receiveBuffer, sizeof(receiveBuffer) - 1, 0);

        if (rbyteCount > 0) {
            receiveBuffer[rbyteCount] = '\0'; 
            cout << "Received data: " << receiveBuffer << endl;

            char buffer[200];
            printf("Enter the message: ");
            cin.getline(buffer, sizeof(buffer));

            if (strcmp(buffer, ":??") == 0) {
                cout << "Server requested to end the session." << endl;
                break;
            }

            int sbyteCount = send(acceptSocket, buffer, strlen(buffer), 0);
            if (sbyteCount == SOCKET_ERROR) {
                cout << "Server send error: " << WSAGetLastError() << endl;
                break;
            }
            else {
                cout << "Server: sent " << sbyteCount << " bytes" << endl;
            }

        }
        else if (rbyteCount == 0) {

            cout << "Connection stopped: client has closed the connection." << endl;
            break; 
        }
        else {

            cout << "Server recv error: " << WSAGetLastError() << endl;
            break; 
        }
    }

  
    closesocket(acceptSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
