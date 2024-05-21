#include "MySocket.hpp"

MySocket::MySocket(uint16_t port)
{
	int optval = 1;

	this->sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
	test_connection(this->sockfd, "Socket");
	test_connection(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)), "Sockoptions");
	this->port = port;
	std::cout << "Puerto" << port << std::endl; 
	bind_socket(port);
	test_connection(listen(this->sockfd, 1000), "Listen"); // no sé que número poner aquí.
	std::cout << "Socket listening\n";
}

MySocket::~MySocket()
{
	if (sockfd > 0)
	{
		close(sockfd);
	}
}

void MySocket::bind_socket(uint16_t port)
{
	setAddress(port);
	std::cout << "Address for port:" << address.sin_family << "\t" << address.sin_port << "\t" << address.sin_addr.s_addr << "\n";
	test_connection(bind(this->sockfd, (sockaddr *)&address, sizeof(address)), "Bind");
	std::cout << "Socket bound\n";
}

void MySocket::test_connection(int value, std::string error)
{
	if (value < 0)
	{
		perror(error.c_str());
		exit(EXIT_FAILURE);
	}
}

int MySocket::getSockfd(void)
{
	return this->sockfd;
}

uint16_t MySocket::getPort(void)
{
	return this->port;
}

sockaddr_in MySocket::getAddress(void)
{
	return this->address;
}

void MySocket::setAddress(uint16_t port)
{
	memset(&address, 0, sizeof(address));
	std::cout << htons(port) << std::endl;
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = INADDR_ANY;
	std::cout << "Address for port:" << address.sin_family << "\t" << address.sin_port << "\t" << address.sin_addr.s_addr << "\n";
}