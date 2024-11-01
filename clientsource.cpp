
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")


using namespace std;

int main() {
    WSADATA wsaData;
    int wserr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wserr = WSAStartup(wVersionRequested, &wsaData);
    if (wserr != 0) {
        cout << "The winsock dll not found" << endl;
        return 0;
    }
    else {
        cout << "The Winsock dll found" << endl;
        cout << "The status: " << wsaData.szSystemStatus << endl;
    }

    SOCKET clientSocket;
    clientSocket = INVALID_SOCKET;
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        cout << "Error at socket(): " << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }
    else {
        cout << "socket is OK!" << endl;
    }

    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &clientService.sin_addr);

    if (inet_pton(AF_INET, "127.0.0.1", &clientService.sin_addr) <= 0) {
        cout << "Invalid address/ Address not supported" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 0;
    }

    clientService.sin_port = htons(55555);
    if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
        cout << "Client: connect() - Failed to connect: " << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }
    else {
        cout << "Client: Connect() is OK!" << endl;
        cout << "Client: Can start sending and receiving data..." << endl;
    }
    
    while (true) {
        char buffer[200];
        printf("Enter the message: ");
        cin.getline(buffer, 200);

        if (strcmp(buffer, ":??") == 0) {
            cout << "Client requested to end the session." << endl;
            break;
        }

        int sbyteCount = send(clientSocket, buffer, strlen(buffer), 0);
        if (sbyteCount == SOCKET_ERROR) {
            cout << "Server send error: " << WSAGetLastError() << endl;
            return -1;
        }

        else {
            cout << "Server: sent " << sbyteCount << endl;
        }

        char receiveBuffer[200];
        int byteCount = recv(clientSocket, receiveBuffer, sizeof(receiveBuffer) - 1, 0);
        if (byteCount > 0) {
            receiveBuffer[byteCount] = '\0'; 
            cout << "Received from server: " << receiveBuffer << endl;
        }
        else if (byteCount == 0) {
            cout << "Connection closed by server." << endl;
            break; 
        }
        else {
            cout << "recv failed: " << WSAGetLastError() << endl;
            break; 
        }
        }
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}