#include "Client.h"

int __cdecl main(int argc, char** argv)
{
	try
	{
		Client client = Client(argc, argv);
		client.process();
	}
	catch (std::string exc)
	{
		std::cout << exc << std::endl;
	}

	return 0;
}