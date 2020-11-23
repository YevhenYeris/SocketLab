#include "Client.h"

Client::Client(int argc, char** argv)
{
    start = std::chrono::system_clock::now();
    of.open(journalPath);

    // Address is not specified
    if (argc != 2) {
        throw("usage: %s server-name " + std::string(argv[0]));
    }

    startWSA();

    // Initialize addrinfo
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Get the address of the server
    iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        WSACleanup();
        throw("getaddrinfo failed with error: " + std::to_string(iResult));
    }

    connectSocket();
}

void Client::startWSA()
{
    //Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        throw("WSAStartup failed with error: " + std::to_string(iResult));
    }
}

void Client::connectSocket()
{
    // Try to connect to the address returned by getaddrinfo
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            WSACleanup();
            WSACleanup();
            throw("socket failed with error: " + std::to_string(WSAGetLastError()));
        }

        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        WSACleanup();
        throw("Unable to connect to server!");
    }
}

void Client::process()
{
    bool finish = false;
    while (!finish)
    {
        std::cin.clear();
        std::string command;

        std::cout << "Enter command:\n ";

        do
        {
            std::getline(std::cin, command);
        } while (command.size() == 0);

        command += '.';
        command = parseCommand(command, finish);

        if (command.substr(0, 7) != "TRYRAND")
        {
            sendAndRecv(command);
        }

    }

    shut();

    of.close();
}

void Client::shut()
{
    // Shutdown the sending side of the client
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        WSACleanup();
        throw("shutdown failed with error: %d\n" + std::to_string(WSAGetLastError()));
    }

    closesocket(ConnectSocket);
    WSACleanup();
}

void Client::sendAndRecv(std::string command)
{
    sendLog(command);
    // Send a request to the server
    iResult = send(ConnectSocket, command.c_str(), (int)strlen(command.c_str()), 0);
    if (iResult == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        WSACleanup();
        throw("send failed with error: " + std::to_string(WSAGetLastError()));
    }

    // Recieve an answer from the server
    iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
    recvLog(std::string(recvbuf));

    if (iResult > 0)
        printResult(recvbuf);

    else if (iResult == 0)
        printf("Connection closed\n");
    else
        printf("recv failed with error: %d\n", WSAGetLastError());
}

void Client::sendLog(std::string str)
{
    std::string message = str.substr(0, str.find('.'));
    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    of << "Send at " << std::ctime(&end_time) << ": " << message << std::endl;
}

void Client::recvLog(std::string str)
{
    std::string message = str.substr(0, str.find('.'));
    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    of << "Recieve at " << std::ctime(&end_time)  << ": " << message << std::endl;
}

void Client::tryRand(int n)
{
    for (int i = 0; i < n; ++i)
    {
        std::string command = "TRY(" + getRandom(i) + ").";
        std::cout << command << "\n";

        sendAndRecv(command);

        if (std::string(recvbuf).substr(0, 6) == "Right!")
            break;
    }
}

void Client::printResult(char* str)
{
    std::string res = "";
    for (int i = 0; i < DEFAULT_BUFLEN; ++i)
    {
        if (str[i] == '.')
            break;
        res += str[i];
    }
    std::cout << res << '\n';
}

std::string Client::getRandom(int i)
{
    std::string res = "";
    srand(time(0) + i);

    for (int i = 0; i < 4; ++i)
    {
        res += std::to_string(rand() % 10);
    }

    return res;
}

std::string Client::parseCommand(std::string str, bool& finish)
{
    if (str.substr(0, 4) == "STOP")
    {
        finish = true;
    }
    if (str.substr(0, 8) == "TRYRAND(" && str[str.length() - 2] == ')')
    {
        if (str.find(".") != std::string::npos)
        {
            std::string number = str.substr(8, str.find(")") + 1);

            int attempts = std::stoi(number);

            tryRand(attempts);
        }
    }
    return str;
}