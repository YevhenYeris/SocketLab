#include "Client.h"

  /*
		TRY(****) - attempt to guess the number
		TRYRAND(N) - attempt with N random numbers generated
		REVEAL - reveal the right number
		RESTART - start with a new number
		STOP - stop and quit
  */

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