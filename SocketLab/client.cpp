// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
//#include <sys/socket.h> 
#include <winsock2.h> 
//#include <unistd.h> 
#include <string.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Wsock32.lib")

#define PORT 8080 

const wchar_t* GetWC(const char* c);

int main(int argc, char const* argv[])
{
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	const char* hello = "Hello from client";
	char buffer[1024] = { 0 };

	WSADATA ws;

	if (FAILED(WSAStartup(MAKEWORD(1, 1), &ws)))
	{
		perror("SOCKET_ERROR");
		exit(EXIT_FAILURE);
	}

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = PORT;

	// Convert IPv4 and IPv6 addresses from text to binary form 
	/*if (InetPtonW(AF_INET, GetWC("192.168.0.1"), &serv_addr.sin_addr) <= 0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}*/

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}
	send(sock, hello, strlen(hello), 0);
	printf("Hello message sent\n");
	valread = recv(sock, buffer, 1024, 0);
	printf("%s\n", buffer);
	return 0;
}

const wchar_t* GetWC(const char* text_char)
{
	size_t length = strlen(text_char);
	wchar_t text_wchar[30];

	mbstowcs_s(&length, text_wchar, text_char, length);

	return text_wchar;
}

/*#define PORT 102512

int main(int argc, char const* argv[])
{
	int s = 0;

	// Объявим переменную для хранения адреса
	sockaddr_in addr;

	// Заполним ее:
	ZeroMemory(&addr, sizeof(addr));
	// тип адреса (TCP/IP)
	addr.sin_family = AF_INET;
	//адрес сервера. Т.к. TCP/IP представляет адреса в числовом виде, то для перевода
	// адреса используем функцию inet_addr.
	addr.sin_addr.S_un.S_addr = inet_addr("193.108.128.226");
	// Порт. Используем функцию htons для перевода номера порта из обычного в //TCP/IP представление.
	addr.sin_port = htons(1234);

	// Дальше выполняем соединение:
	if (SOCKET_ERROR == (connect(s, (sockaddr*)&addr, sizeof(addr))))
	{
		perror("SOCKET_ERROR");
	}

	char buff[512] = { 0 };

	if (SOCKET_ERROR == (send(s, (char*)&buff, 512, 0)))
	{
		perror("SOCKET_ERROR");
	}

	return 0;
}*/