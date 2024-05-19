#include "MySocket.hpp"

MySocket::MySocket(uint16_t port)
{
	this->sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
	this->port = htons(port);
	test_connection(this->sockfd, "Socket: ");
	bind_socket(port);
	test_connection(listen(this->sockfd, 1000), "Listen: "); // no sé que número poner aquí.
}

MySocket::~MySocket()
{
	if (sockfd > 0)
		close(sockfd);
}

void MySocket::bind_socket(uint16_t port)
{
	setAddress(port);
	test_connection(bind(this->sockfd, (sockaddr *)&address, sizeof(address)), "Bind: ");
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

sockaddr_in	MySocket::getAddress(void)
{
	return this->address;
}

void MySocket::setAddress(uint16_t port)
{
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = port;
	address.sin_addr.s_addr = INADDR_ANY;
}