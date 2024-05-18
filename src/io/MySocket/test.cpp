#include "MySocket.hpp"

int main()
{
	MySocket         sockets(AF_INET, SOCK_STREAM, IPPROTO_TCP, 8080, "localhost"); // port < 1024 requieren root.
	struct addrinfo *lp;

	std::cout << "Socket fd:" << sockets.getSockfd() << std::endl;
	std::cout << lp << std::endl;
	for (lp = sockets.getAddress(); lp != NULL; lp = lp->ai_next)
	{
		std::cout << lp->ai_family << "\t" << std::endl;
	}
	return 0;
}