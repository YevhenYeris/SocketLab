#include "Server.h"

Server::Server()
{
    of.open(journalPath);

    startWSA();

    // Initialize addrinfo
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Get the address
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        WSACleanup();
        throw("getaddrinfo failed with error: " + std::to_string(iResult));
    }

    setListenSocket();
}

void Server::startWSA()
{
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        throw("WSAStartup failed with error: " + std::to_string(iResult));
    }
}

void Server::setListenSocket()
{
    // Create listening socket
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        freeaddrinfo(result);
        WSACleanup();
        throw("socket failed with error: " + std::to_string(WSAGetLastError()));
    }

    // Bind the socket with th address
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        throw("bind failed with error: " + std::to_string(WSAGetLastError()));
    }

    // Address has been used and is no longer needed
    freeaddrinfo(result);

    // Set the socket listening on its IP
    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        closesocket(ListenSocket);
        WSACleanup();
        throw("listen failed with error: " + std::to_string(WSAGetLastError()));
    }

    // Accept a connection
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        closesocket(ListenSocket);
        WSACleanup();
        throw("accept failed with error: " + std::to_string(WSAGetLastError()));
    }

    // Whe ClientSocket is set ListenSocket is no longer needed
    closesocket(ListenSocket);
}

void Server::process()
{
    Game game = Game();

    do {
        recieve();

        if (iResult > 0)
        {
            std::string req = getCommand(std::string(recvbuf), game, iResult);

            sendRequest(req);
        }
        else if (iResult == 0)
            printf("Connection closing...\n");
        else {
            closesocket(ClientSocket);
            WSACleanup();
            throw("recv failed with error: " + std::to_string(WSAGetLastError()));
        }

    } while (iResult > 0);

    shut();
}

void Server::recieve()
{
    // Recieve request from the client
    iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
    recvLog(std::string(recvbuf));
}

void Server::sendRequest(std::string req)
{
    sendLog(req);

    // Send request to the client
    iSendResult = send(ClientSocket, req.c_str(), (int)strlen(req.c_str()), 0);

    if (iSendResult == SOCKET_ERROR) {
        closesocket(ClientSocket);
        WSACleanup();
        throw("send failed with error: " + std::to_string(WSAGetLastError()));
    }
}

void Server::shut()
{
    // Shutdown the connection with the client
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        closesocket(ClientSocket);
        WSACleanup();
        throw("shutdown failed with error: " + std::to_string(WSAGetLastError()));
    }

    closesocket(ClientSocket);
    WSACleanup();

    of.close();
}

void Server::sendLog(std::string str)
{
    std::string message = str.substr(0, str.find('.'));

    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    of << "Send at " << std::ctime(&end_time) << ": " << message << std::endl;
}

void Server::recvLog(std::string str)
{
    std::string message = str.substr(0, str.find('.'));

    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    of << "Recieve at " << std::ctime(&end_time) << ": " << message << std::endl;
}

std::string getRandom(int i)
{
    std::string res = "";
    srand(i);

    for (int i = 0; i < 4; ++i)
    {
        res += std::to_string(rand() % 10);
    }

    return res;
}

std::string Server::getCommand(std::string str, Game& game, int& iResult)
{
    if (str.substr(0, 3) == "WHO")
    {
        return "Yeris Yevhen, K-25\nVar 18: guess a 4-digit number.";
    }
    if (str.substr(0, 4) == "STOP")
    {
        shut();
        return " Program stopped\nright: " + game.getNumber() + '.';
    }
    if (str.substr(0, 7) == "RESTART")
    {
        game = Game();
        return " Game restarted.";
    }
    if (str.substr(0, 6) == "REVEAL")
    {
        std::string right = game.getNumber();
        return " Number: " + right + '.';
    }
    if (str.substr(0, 3) == "TRY")
    {
        if (str[3] == '(' && str[8] == ')')
        {
            attempts = 1;
            game.setEntered(str.substr(4, 4));
            return game.getResult() + '.';
        }
    }

    return "Unknown command.";
}