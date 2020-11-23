#pragma once
#undef UNICODE
#define WIN32_LEAN_AND_MEAN
#define  _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include "Game.h"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 100
#define DEFAULT_PORT "102518"

class Server
{
private:
    std::ofstream of;
    std::string journalPath = "D:/Shkola/SocketLab/serverJournal.txt";

    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    int attempts = 0;

	std::string getCommand(std::string str, Game& game, int& iResult);

    void startWSA();

    void setListenSocket();

    void recieve();

    void sendRequest(std::string req);

public:
    void process();

    void shut();

    void sendLog(std::string str);
    void recvLog(std::string str);

    Server();
};

