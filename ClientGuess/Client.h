#pragma once
#define WIN32_LEAN_AND_MEAN
#define  _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <random>
#include <fstream>
#include <ctime>
#include <exception>
#include <chrono>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 100
#define DEFAULT_PORT "102518"

class Client
{
private:
    std::ofstream of;
    std::string journalPath = "D:/Shkola/SocketLab/clientJournal.txt";

    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;

    struct addrinfo* result = NULL,
        * ptr = NULL,
        hints;

    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;

    std::chrono::system_clock::time_point start;

    void startWSA();

    void connectSocket();

    void shut();

    void printResult(char* str);

    std::string getRandom(int i);

    std::string parseCommand(std::string str, bool& finish);

public:
    Client(int argc, char** argv);

    void process();

    void sendLog(std::string str);
    void recvLog(std::string str);

    void sendAndRecv(std::string command);

    void tryRand(int n);
};

