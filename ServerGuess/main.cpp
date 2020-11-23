#include "Server.h"

int __cdecl main(void)
{
	try
	{
		Server server = Server();
		server.process();
	}
	catch (std::string exc)
	{
		std::cout << exc << std::endl;
	}
	return 0;
}