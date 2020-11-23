#include "Server.h"

/*
	  TRY(****) - attempt to guess the number
	  TRYRAND(N) - attempt with N random numbers generated
	  REVEAL - reveal the right number
	  RESTART - start with a new number
	  STOP - stop and quit
*/

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