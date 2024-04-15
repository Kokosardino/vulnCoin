#include "CServer.h"

int main(int argc, char **argv) 
{
	CServer server;

	if(argc < 2)
	{
		std::cerr << rang::fg::red << rang::style::bold << "Port number not specified, exiting!" << rang::style::reset << std::endl;
		return 1;
	}

	if(server.bindSocket(std::stoi(std::string(argv[1]))) == 0)
	{
		server.run();
	} else 
	{		
		return 1;
	}
		
	return 0;
}
